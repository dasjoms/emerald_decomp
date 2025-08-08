#ifndef PLATFORM_PC_INPUT_H
#define PLATFORM_PC_INPUT_H

#include <stdbool.h>
#include "gba/types.h"

void InputPoll(void);
u16 InputGetKeys(void);
bool InputShouldQuit(void);

#endif // PLATFORM_PC_INPUT_H
