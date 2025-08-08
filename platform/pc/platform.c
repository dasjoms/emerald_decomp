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
