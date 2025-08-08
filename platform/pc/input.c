#include "input.h"

static u16 sKeyState;

void InputPoll(void)
{
    sKeyState = 0;
}

u16 InputGetKeys(void)
{
    return sKeyState;
}
