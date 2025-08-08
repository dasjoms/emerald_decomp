#include "input.h"
#include <SDL.h>
#include "gba/io_reg.h"

static u16 sKeyState;
static bool sQuit;

static void SetKey(u16 mask, bool down)
{
    if (down)
        sKeyState |= mask;
    else
        sKeyState &= ~mask;
}

void InputPoll(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            sQuit = true;
        else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        {
            bool down = (e.type == SDL_KEYDOWN);
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_Z:        SetKey(A_BUTTON, down); break;
            case SDL_SCANCODE_X:        SetKey(B_BUTTON, down); break;
            case SDL_SCANCODE_BACKSPACE:SetKey(SELECT_BUTTON, down); break;
            case SDL_SCANCODE_RETURN:   SetKey(START_BUTTON, down); break;
            case SDL_SCANCODE_RIGHT:    SetKey(DPAD_RIGHT, down); break;
            case SDL_SCANCODE_LEFT:     SetKey(DPAD_LEFT, down); break;
            case SDL_SCANCODE_UP:       SetKey(DPAD_UP, down); break;
            case SDL_SCANCODE_DOWN:     SetKey(DPAD_DOWN, down); break;
            case SDL_SCANCODE_A:        SetKey(L_BUTTON, down); break;
            case SDL_SCANCODE_S:        SetKey(R_BUTTON, down); break;
            }
        }
    }
}

u16 InputGetKeys(void)
{
    return sKeyState;
}

bool InputShouldQuit(void)
{
    return sQuit;
}
