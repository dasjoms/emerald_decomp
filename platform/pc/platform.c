#include "platform.h"
#include "video.h"
#include "audio.h"
#include "input.h"
#include "timer.h"
#include "fs.h"
#include <SDL.h>
#include <SDL_image.h>

bool PlatformInit(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
        return false;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Quit();
        return false;
    }

    if (!VideoInit() || !AudioInit() || !TimerInit())
    {
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    return true;
}

bool PlatformRunFrame(void)
{
    InputPoll();
    VideoUpdate();
    return !InputShouldQuit();
}

void PlatformShutdown(void)
{
    AudioShutdown();
    VideoShutdown();
    TimerShutdown();
    IMG_Quit();
    SDL_Quit();
}

void PlatformSetBackdropColor(u16 color)
{
    VideoSetBackdropColor(color);
}

void PlatformSetWaitCnt(u16 value)
{
    (void)value;
}

void PlatformInitInterrupts(void)
{
}

void PlatformWaitForVBlank(void)
{
    TimerWaitVBlank();
}
