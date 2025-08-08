#include "platform.h"
#include "video.h"
#include "audio.h"
#include "input.h"
#include "timer.h"
#include "fs.h"

bool PlatformInit(void)
{
    return VideoInit() && AudioInit() && TimerInit();
}

bool PlatformRunFrame(void)
{
    InputPoll();
    VideoUpdate();
    return true;
}

void PlatformShutdown(void)
{
    AudioShutdown();
    VideoShutdown();
    TimerShutdown();
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
