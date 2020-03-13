/*
 * OpenAL Helpers
 *
 * Copyright (c) 2011 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* This file contains routines to help with some menial OpenAL-related tasks,
 * such as opening a device and setting up a context, closing the device and
 * destroying its context, converting between frame counts and byte lengths,
 * finding an appropriate buffer format, and getting readable strings for
 * channel configs and sample types. */

#include "alhelpers.h"
#include <iostream>
#include <cerrno>
#include <cstring>

#include "AL/al.h"
#include "AL/alc.h"


/* InitAL opens a device and sets up a context using default attributes, making
 * the program ready to call OpenAL functions. */
namespace osu {

    int avcodec_to_openal_format_name(int chanels, AVSampleFormat format) {
        if (chanels == 1) {
            switch (format) {
                case AV_SAMPLE_FMT_U8:
                    //todo
                    break;
            }
        }
    }

    int InitAL(ALCdevice *device, ALCcontext *context, const char *name) {
        /* Open and initialize a device */
        device = alcOpenDevice(name);
        if (!device) {
            fprintf(stderr, "Could not open a device!\n");
            return 1;
        }

        context = alcCreateContext(device, nullptr);
        if (context == nullptr || alcMakeContextCurrent(context) == ALC_FALSE) {
            if (context != nullptr)
                alcDestroyContext(context);
            alcCloseDevice(device);
            fprintf(stderr, "Could not set a context!\n");
            return 1;
        }
        return true;

        return 0;
    }

/* CloseAL closes the device belonging to the current context, and destroys the
 * context. */
    void CloseAL() {
        ALCdevice *device;
        ALCcontext *ctx;

        ctx = alcGetCurrentContext();
        if (ctx == NULL)
            return;

        device = alcGetContextsDevice(ctx);

        alcMakeContextCurrent(NULL);
        alcDestroyContext(ctx);
        alcCloseDevice(device);
    }

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

    int altime_get(void)
    {
        static int start_time = 0;
        int cur_time;
        union {
            FILETIME ftime;
            ULARGE_INTEGER ulint;
        } systime;
        GetSystemTimeAsFileTime(&systime.ftime);
        /* FILETIME is in 100-nanosecond units, or 1/10th of a microsecond. */
        cur_time = (int)(systime.ulint.QuadPart/10000);

        if(!start_time)
            start_time = cur_time;
        return cur_time - start_time;
    }

    void al_nssleep(unsigned long nsec)
    {
        Sleep(nsec / 1000000);
    }

#else

#include <ctime>
#include <unistd.h>

    int altime_get() {
        static int start_time = 0u;
        int cur_time;

#if _POSIX_TIMERS > 0
        struct timespec ts;
        int ret = clock_gettime(CLOCK_REALTIME, &ts);
        if (ret != 0) return 0;
        cur_time = (int) (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else /* _POSIX_TIMERS > 0 */
        struct timeval tv;
        int ret = gettimeofday(&tv, NULL);
        if(ret != 0) return 0;
        cur_time = (int)(tv.tv_sec*1000 + tv.tv_usec/1000);
#endif

        if (!start_time)
            start_time = cur_time;
        return cur_time - start_time;
    }

    void al_nssleep(unsigned long nsec) {
        struct timespec ts, rem;
        ts.tv_sec = (time_t) (nsec / 1000000000ul);
        ts.tv_nsec = (long) (nsec % 1000000000ul);
        while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
            ts = rem;
    }

#endif
}