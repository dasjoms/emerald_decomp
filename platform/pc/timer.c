#include "timer.h"
#include <SDL.h>
#include "gba/types.h"

static u32 sFrameStart;

bool TimerInit(void)
{
    sFrameStart = SDL_GetTicks();
    return true;
}

void TimerWaitVBlank(void)
{
    u32 now = SDL_GetTicks();
    u32 elapsed = now - sFrameStart;
    const u32 frameMs = 1000 / 60;
    if (elapsed < frameMs)
        SDL_Delay(frameMs - elapsed);
    sFrameStart = SDL_GetTicks();
}

void TimerShutdown(void)
{
}
