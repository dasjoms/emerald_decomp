#include "audio.h"
#include <SDL.h>
#include <string.h>
#include "m4a.h"

static SDL_AudioDeviceID sAudioDevice;
struct SoundInfo *gSoundInfoPtr;

static void AudioCallback(void *userdata, Uint8 *stream, int len)
{
    (void)userdata;
    while (len > 0)
    {
        m4aSoundMain();
        int copy = PCM_DMA_BUF_SIZE;
        if (copy > len)
            copy = len;
        memcpy(stream, gSoundInfo.pcmBuffer, copy);
        stream += copy;
        len -= copy;
    }
}

bool AudioInit(void)
{
    SDL_AudioSpec want = {0};
    want.freq = 13379;
    want.format = AUDIO_S8;
    want.channels = 2;
    want.samples = 512;
    want.callback = AudioCallback;

    sAudioDevice = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    return sAudioDevice != 0;
}

void AudioStart(void)
{
    if (sAudioDevice != 0)
        SDL_PauseAudioDevice(sAudioDevice, 0);
}

void AudioShutdown(void)
{
    if (sAudioDevice != 0)
        SDL_CloseAudioDevice(sAudioDevice);
    sAudioDevice = 0;
}

