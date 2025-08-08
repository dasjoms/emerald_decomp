#include "assets.h"
#include <SDL.h>
#include <stdlib.h>

void *AssetsLoadFile(const char *path, size_t *size)
{
    if (size)
        *size = 0;

    SDL_RWops *rw = SDL_RWFromFile(path, "rb");
    if (!rw)
        return NULL;

    Sint64 len = SDL_RWsize(rw);
    if (len <= 0)
    {
        SDL_RWclose(rw);
        return NULL;
    }

    void *buffer = malloc((size_t)len);
    if (!buffer)
    {
        SDL_RWclose(rw);
        return NULL;
    }

    if (SDL_RWread(rw, buffer, 1, (size_t)len) != (size_t)len)
    {
        free(buffer);
        SDL_RWclose(rw);
        return NULL;
    }

    SDL_RWclose(rw);
    if (size)
        *size = (size_t)len;
    return buffer;
}

void AssetsFreeFile(void *buffer)
{
    free(buffer);
}
