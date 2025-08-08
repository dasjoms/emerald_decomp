#ifndef PLATFORM_PC_VIDEO_H
#define PLATFORM_PC_VIDEO_H

#include <stdbool.h>
#include "gba/types.h"

bool VideoInit(void);
void VideoUpdate(void);
void VideoShutdown(void);
void VideoSetBackdropColor(u16 color);

#endif // PLATFORM_PC_VIDEO_H
