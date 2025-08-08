#ifndef PLATFORM_PC_TIMER_H
#define PLATFORM_PC_TIMER_H

#include <stdbool.h>

bool TimerInit(void);
void TimerWaitVBlank(void);
void TimerShutdown(void);

#endif // PLATFORM_PC_TIMER_H
