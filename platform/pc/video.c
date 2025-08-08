#include "video.h"
#include <SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define WINDOW_SCALE 2

static SDL_Window *sWindow;
static SDL_Renderer *sRenderer;
static SDL_Texture *sTexture;
static u32 *sFramebuf;

static u32 GbaColorToArgb(u16 color)
{
    u8 r = (color & 0x1F) * 8;
    u8 g = ((color >> 5) & 0x1F) * 8;
    u8 b = ((color >> 10) & 0x1F) * 8;
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

bool VideoInit(void)
{
    sWindow = SDL_CreateWindow("Pokemon Emerald", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE, 0);
    if (!sWindow)
        return false;

    sRenderer = SDL_CreateRenderer(sWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!sRenderer)
    {
        SDL_DestroyWindow(sWindow);
        return false;
    }

    sTexture = SDL_CreateTexture(sRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                 SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!sTexture)
    {
        SDL_DestroyRenderer(sRenderer);
        SDL_DestroyWindow(sWindow);
        return false;
    }

    sFramebuf = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u32));
    if (!sFramebuf)
    {
        SDL_DestroyTexture(sTexture);
        SDL_DestroyRenderer(sRenderer);
        SDL_DestroyWindow(sWindow);
        return false;
    }

    VideoSetBackdropColor(0);
    return true;
}

void VideoUpdate(void)
{
    SDL_UpdateTexture(sTexture, NULL, sFramebuf, SCREEN_WIDTH * sizeof(u32));
    SDL_RenderClear(sRenderer);
    SDL_RenderCopy(sRenderer, sTexture, NULL, NULL);
    SDL_RenderPresent(sRenderer);
}

void VideoShutdown(void)
{
    free(sFramebuf);
    SDL_DestroyTexture(sTexture);
    SDL_DestroyRenderer(sRenderer);
    SDL_DestroyWindow(sWindow);
}

void VideoSetBackdropColor(u16 color)
{
    u32 argb = GbaColorToArgb(color);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        sFramebuf[i] = argb;
}
