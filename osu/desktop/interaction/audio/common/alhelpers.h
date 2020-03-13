#ifndef ALHELPERS_H
#define ALHELPERS_H
extern "C" {
#include "libavutil/samplefmt.h"
}

#include "AL/alc.h"
#include "AL/al.h"

namespace osu {
/* Some helper functions to get the name from the format enums. */
    const char *FormatName(ALenum type);

/* Easy device init/deinit functions. InitAL returns 0 on success. */
    int InitAL(ALCdevice *device, ALCcontext *context, const char* name);

    void CloseAL();

/* Cross-platform timeget and sleep functions. */
    int altime_get();

    void al_nssleep(unsigned long nsec);

    int avcodec_to_openal_format_name(int chanels, AVSampleFormat format);

#endif /* ALHELPERS_H */
}