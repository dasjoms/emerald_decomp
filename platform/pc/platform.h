#ifndef PLATFORM_PC_PLATFORM_H
#define PLATFORM_PC_PLATFORM_H

#include <stdbool.h>
#include "gba/types.h"

bool PlatformInit(void);
bool PlatformRunFrame(void);
void PlatformShutdown(void);
void PlatformSetBackdropColor(u16 color);
void PlatformSetWaitCnt(u16 value);
void PlatformInitInterrupts(void);
void PlatformWaitForVBlank(void);

#endif // PLATFORM_PC_PLATFORM_H
