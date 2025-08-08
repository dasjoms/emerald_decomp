#ifndef PLATFORM_PC_PLATFORM_H
#define PLATFORM_PC_PLATFORM_H

#include <stdbool.h>

bool PlatformInit(void);
bool PlatformRunFrame(void);
void PlatformShutdown(void);

#endif // PLATFORM_PC_PLATFORM_H
