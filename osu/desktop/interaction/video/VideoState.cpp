//
// Created by Masterlogick on 3/8/20.
//

#include "VideoState.h"
#include "../../graphics/opengl/UtillDrawer.h"
#include "../../graphics/opengl/Shader.h"
#include <iostream>
#include <algorithm>

extern "C" {
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}
#define VIDEO_FRAME_WIDTH 1280
#define VIDEO_FRAME_HEIGHT 720
#define IMAGE_DATA_SIZE VIDEO_FRAME_WIDTH * VIDEO_FRAME_HEIGHT
namespace osu {
    const GLbitfield bufferStorageBitfield = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;
    const GLbitfield bufferMappingBitfield = GL_WRITE_ONLY | GL_MAP_INVALIDATE_BUFFER_BIT /*| GL_MAP_FLUSH_EXPLICIT_BIT*/;

    static std::chrono::microseconds get_avtime() { return std::chrono::microseconds{av_gettime()}; }

    VideoState::~VideoState() {
        //todo delete another opengl objects

        if (mImage)
            SDL_DestroyTexture(mImage);
        mImage = nullptr;
    }

    std::chrono::nanoseconds VideoState::getClock() {
        // NOTE: This returns incorrect times while not playing.
        std::lock_guard<std::mutex> _{mDispPtsMutex};
        if (mDisplayPtsTime == std::chrono::microseconds::min())
            return std::chrono::nanoseconds::zero();
        auto delta = get_avtime() - mDisplayPtsTime;
        return mDisplayPts + delta;
    }

// Called by VideoState::updateVideo to display the next video frame.
    void VideoState::display(SDL_Window *screen, SDL_Renderer *renderer) {
        if (!mImage)
            return;

        double aspect_ratio;
        int win_w, win_h;
        int w, h, x, y;

        if (mCodecCtx->sample_aspect_ratio.num == 0)
            aspect_ratio = 0.0;
        else {
            aspect_ratio = av_q2d(mCodecCtx->sample_aspect_ratio) * mCodecCtx->width /
                           mCodecCtx->height;
        }
        if (aspect_ratio <= 0.0)
            aspect_ratio = static_cast<double>(mCodecCtx->width) / mCodecCtx->height;

        SDL_GetWindowSize(screen, &win_w, &win_h);
        h = win_h;
        w = (static_cast<int>(std::rint(h * aspect_ratio)) + 3) & ~3;
        if (w > win_w) {
            w = win_w;
            h = (static_cast<int>(std::rint(w / aspect_ratio)) + 3) & ~3;
        }
        x = (win_w - w) / 2;
        y = (win_h - h) / 2;

        SDL_Rect src_rect{0, 0, mWidth, mHeight};
        SDL_Rect dst_rect{x, y, w, h};
        SDL_RenderCopy(renderer, mImage, &src_rect, &dst_rect);
        SDL_RenderPresent(renderer);
    }

// Called regularly on the main thread where the SDL_Renderer was created. It
// handles updating the textures of decoded frames and displaying the latest
// frame.

    void VideoState::updateVideo(SDL_Window *screen, SDL_Renderer *renderer) {
        size_t read_idx{mPictQRead};
        Picture *vp{&mPictQ[read_idx]};

        auto clocktime = mMovie->getMasterClock();
        bool updated{false};
        while (1) {
            size_t next_idx{(read_idx + 1) % mPictQ.size()};
            if (next_idx == mPictQWrite)
                break;
            Picture *nextvp{&mPictQ[next_idx]};
            if (clocktime < nextvp->mPts) {
                break;
            }
            vp = nextvp;
            updated = true;
            read_idx = next_idx;
        }
        if (mMovie->mQuit) {
            if (mEOS)
                mFinalUpdate = true;
            mPictQRead = read_idx;
            std::unique_lock<std::mutex>{mPictQMutex}.unlock();
            mPictQCond.notify_one();
            return;
        }

        if (updated) {
            mPictQRead = read_idx;
            std::unique_lock<std::mutex>{mPictQMutex}.unlock();
            mPictQCond.notify_one();

            /* allocate or resize the buffer! */
            bool fmt_updated{false};
            if (!mImage /*|| mWidth != mCodecCtx->width || mHeight != mCodecCtx->height*/) {
                fmt_updated = true;
                if (mImage)
                    SDL_DestroyTexture(mImage);
                mImage = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                           mCodecCtx->coded_width, mCodecCtx->coded_height);
                if (!mImage)
                    std::cerr << "Failed to create YV12 texture!" << std::endl;
                mWidth = VIDEO_FRAME_WIDTH;//mCodecCtx->width;
                mHeight = VIDEO_FRAME_HEIGHT;//mCodecCtx->height;

                if (mFirstUpdate && mWidth > 0 && mHeight > 0) {
                    /* For the first update, set the window size to the video size. */
                    mFirstUpdate = false;

                    int w{mWidth};
                    int h{mHeight};
                    if (mCodecCtx->sample_aspect_ratio.den != 0) {
                        double aspect_ratio = av_q2d(mCodecCtx->sample_aspect_ratio);
                        if (aspect_ratio >= 1.0)
                            w = static_cast<int>(w * aspect_ratio + 0.5);
                        else if (aspect_ratio > 0.0)
                            h = static_cast<int>(h / aspect_ratio + 0.5);
                    }
                    SDL_SetWindowSize(screen, w, h);
                }
            }
            if (!mSwscaleCtx || fmt_updated) {
                mSwscaleCtx = sws_getContext(
                        mCodecCtx->width, mCodecCtx->height, mCodecCtx->pix_fmt,
                        VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, AV_PIX_FMT_YUV420P, 0,
                        nullptr, nullptr, nullptr
                );
            }
            AVFrame *frame{vp->mFrame};
            uint8_t *pict_data[3];
            int pict_linesize[3];
            int coded_w{mCodecCtx->coded_width};
            int coded_h{mCodecCtx->coded_height};
            int w{mCodecCtx->width};
            int h{mCodecCtx->height};
            pict_linesize[0] = VIDEO_FRAME_WIDTH;
            pict_linesize[1] = VIDEO_FRAME_WIDTH / 2;
            pict_linesize[2] = VIDEO_FRAME_WIDTH / 2;
            pboLock.lock();
            pict_data[0] = reinterpret_cast<uint8_t *>(mappedPBO[nextIndex * 3]);
            pict_data[1] = reinterpret_cast<uint8_t *>(mappedPBO[nextIndex * 3 + 1]);
            pict_data[2] = reinterpret_cast<uint8_t *>(mappedPBO[nextIndex * 3 + 2]);
            sws_scale(mSwscaleCtx,
                      reinterpret_cast<uint8_t **>(frame->data), frame->linesize, 0, h, pict_data, pict_linesize);
            pboLock.unlock();
            if (mImage) {

                void *pixels{nullptr};
                int pitch{0};
                if (SDL_LockTexture(mImage, nullptr, &pixels, &pitch) != 0) {
                    std::cerr << "Failed to lock texture" << std::endl;
                } else {
                    pict_data[0] = static_cast<uint8_t *>(pixels);
                    pict_data[1] = pict_data[0] + coded_w * coded_h;
                    pict_data[2] = pict_data[1] + coded_w * coded_h / 4;

                    pict_linesize[0] = pitch;
                    pict_linesize[1] = pitch / 2;
                    pict_linesize[2] = pitch / 2;

                    sws_scale(mSwscaleCtx,
                              reinterpret_cast<uint8_t **>(frame->data), frame->linesize, 0, h, pict_data, pict_linesize);
                    SDL_UnlockTexture(mImage);
                }
            }
            redraw = true;
        }

        if (redraw) {
            // Show the picture!
            display(screen, renderer);
        }

        if (updated) {
            auto disp_time = get_avtime();

            std::lock_guard<std::mutex> _{mDispPtsMutex};
            mDisplayPts = vp->mPts;
            mDisplayPtsTime = disp_time;
        }
        if (mEOS) {
            if ((read_idx + 1) % mPictQ.size() == mPictQWrite) {
                mFinalUpdate = true;
                std::unique_lock<std::mutex>{mPictQMutex}.unlock();
                mPictQCond.notify_one();
            }
        }
    }

    int VideoState::handler() {
        std::for_each(mPictQ.begin(), mPictQ.end(),
                      [](Picture &pict) -> void { pict.mFrame = av_frame_alloc(); });

        // Prefill the codec buffer.
        do {
            const int ret{mPackets.sendTo(mCodecCtx)};
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
        } while (1);

        {
            std::lock_guard<std::mutex> _{mDispPtsMutex};
            mDisplayPtsTime = get_avtime();
        }

        auto current_pts = std::chrono::nanoseconds::zero();
        while (!mMovie->mQuit) {
            size_t write_idx{mPictQWrite};
            Picture *vp{&mPictQ[write_idx]};

            // Retrieve video frame.
            AVFrame *decoded_frame{vp->mFrame};
            int ret;
            while ((ret = avcodec_receive_frame(mCodecCtx, decoded_frame)) == AVERROR(EAGAIN))
                mPackets.sendTo(mCodecCtx);
            if (ret != 0) {
                if (ret == AVERROR_EOF) break;
                std::cerr << "Failed to receive frame: " << ret << std::endl;
                continue;
            }

            // Get the PTS for this frame.
            if (decoded_frame->best_effort_timestamp != AV_NOPTS_VALUE)
                current_pts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::duration<double>{av_q2d(mStream->time_base) * decoded_frame->best_effort_timestamp});
            vp->mPts = current_pts;

            // Update the video clock to the next expected PTS.
            auto frame_delay = av_q2d(mCodecCtx->time_base);
            frame_delay += decoded_frame->repeat_pict * (frame_delay * 0.5);
            current_pts += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>{frame_delay});

            // Put the frame in the queue to be loaded into a texture and displayed
            // by the rendering thread.

            write_idx = (write_idx + 1) % mPictQ.size();
            mPictQWrite = write_idx;

            // Send a packet now so it's hopefully ready by the time it's needed.
            mPackets.sendTo(mCodecCtx);
            if (write_idx == mPictQRead) {
                // Wait until we have space for a new pic
                std::unique_lock<std::mutex> lock{mPictQMutex};
                while (write_idx == mPictQRead &&
                       !mMovie->mQuit)
                    mPictQCond.wait(lock);
            }
        }
        mEOS = true;

        std::unique_lock<std::mutex> lock{mPictQMutex};
        while (!mFinalUpdate) mPictQCond.wait(lock);

        return 0;
    }

    void VideoState::draw(float x, float y) {
        if (redraw) {
            pboLock.lock();
            index = (index + 1) & 1;
            nextIndex = (index + 1) & 1;
            glUnmapNamedBuffer(unpackPBO[index * 3]);
            glUnmapNamedBuffer(unpackPBO[index * 3 + 1]);
            glUnmapNamedBuffer(unpackPBO[index * 3 + 2]);
            glBindTexture(GL_TEXTURE_2D, yPlaneTexture);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, unpackPBO[index * 3]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, uPlaneTexture);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, unpackPBO[index * 3 + 1]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VIDEO_FRAME_WIDTH / 2, VIDEO_FRAME_HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, vPlaneTexture);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, unpackPBO[index * 3 + 2]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VIDEO_FRAME_WIDTH / 2, VIDEO_FRAME_HEIGHT / 2, GL_RED, GL_UNSIGNED_BYTE, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            mappedPBO[index * 3] = static_cast<float *>(glMapNamedBuffer(unpackPBO[index * 3], bufferMappingBitfield));
            mappedPBO[index * 3 + 1] = static_cast<float *>(glMapNamedBuffer(unpackPBO[index * 3 + 1], bufferMappingBitfield));
            mappedPBO[index * 3 + 2] = static_cast<float *>(glMapNamedBuffer(unpackPBO[index * 3 + 2], bufferMappingBitfield));
            redraw = false;
            pboLock.unlock();
        }
        float vertexes[] = {
                x, y, 0, 1,
                x + VIDEO_FRAME_WIDTH, y, 1, 1,
                x, y + VIDEO_FRAME_HEIGHT, 0, 0,
                x + VIDEO_FRAME_WIDTH, y + VIDEO_FRAME_HEIGHT, 1, 0
        };
        glNamedBufferSubData(drawVBO, 0, 4 * 4 * sizeof(float), vertexes);
        glBindVertexArray(drawVAO);
        glBindTextureUnit(0, yPlaneTexture);
        glBindTextureUnit(1, uPlaneTexture);
        glBindTextureUnit(2, vPlaneTexture);
        Shader::videoDrawingShader->bind();
        Shader::videoDrawingShader->uniform("yPlaneSampler", 0);
        Shader::videoDrawingShader->uniform("uPlaneSampler", 1);
        Shader::videoDrawingShader->uniform("vPlaneSampler", 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void VideoState::initialise() {
        glCreateTextures(GL_TEXTURE_2D, 1, &yPlaneTexture);
        glCreateTextures(GL_TEXTURE_2D, 1, &uPlaneTexture);
        glCreateTextures(GL_TEXTURE_2D, 1, &vPlaneTexture);

        glTextureStorage2D(yPlaneTexture, 1, GL_R8, VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT);
        glTextureStorage2D(uPlaneTexture, 1, GL_R8, VIDEO_FRAME_WIDTH / 2, VIDEO_FRAME_HEIGHT / 2);
        glTextureStorage2D(vPlaneTexture, 1, GL_R8, VIDEO_FRAME_WIDTH / 2, VIDEO_FRAME_HEIGHT / 2);

        glCreateBuffers(PBO_AMOUNT, unpackPBO);
        for (int i = 0; i < PBO_AMOUNT; i += 3) {
            glNamedBufferStorage(unpackPBO[i], IMAGE_DATA_SIZE, nullptr, bufferStorageBitfield);
            glNamedBufferStorage(unpackPBO[i + 1], IMAGE_DATA_SIZE / 4, nullptr, bufferStorageBitfield);
            glNamedBufferStorage(unpackPBO[i + 2], IMAGE_DATA_SIZE / 4, nullptr, bufferStorageBitfield);
            mappedPBO[i] =
                    static_cast<float *>(glMapNamedBuffer(unpackPBO[i], bufferMappingBitfield));
            mappedPBO[i + 1] =
                    static_cast<float *>(glMapNamedBuffer(unpackPBO[i + 1], bufferMappingBitfield));
            mappedPBO[i + 2] =
                    static_cast<float *>(glMapNamedBuffer(unpackPBO[i + 2], bufferMappingBitfield));
        }

        glCreateVertexArrays(1, &drawVAO);
        glCreateBuffers(1, &drawVBO);

        glNamedBufferData(drawVBO, 4 * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

        GLuint positionLocation = Shader::videoDrawingShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::videoDrawingShader->getAttribLocation("texCord");

        glEnableVertexArrayAttrib(drawVAO, positionLocation);
        glVertexArrayAttribFormat(drawVAO, positionLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(drawVAO, positionLocation, drawVBO, 0, 2 * 2 * sizeof(float));

        glEnableVertexArrayAttrib(drawVAO, texCordLocation);
        glVertexArrayAttribFormat(drawVAO, texCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(drawVAO, texCordLocation, drawVBO, 2 * sizeof(float), 2 * 2 * sizeof(float));
    }
}