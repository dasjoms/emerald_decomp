#include "assets.h"
#include <SDL_image.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct FileCacheEntry
{
    char *path;
    void *data;
    size_t size;
    struct FileCacheEntry *next;
} FileCacheEntry;

typedef struct PngCacheEntry
{
    char *path;
    SDL_Surface *surface;
    struct PngCacheEntry *next;
} PngCacheEntry;

typedef struct PalCacheEntry
{
    char *path;
    u16 *colors;
    size_t count;
    struct PalCacheEntry *next;
} PalCacheEntry;

static FileCacheEntry *sFileCache;
static PngCacheEntry *sPngCache;
static PalCacheEntry *sPalCache;

void *AssetsLoadFile(const char *path, size_t *size)
{
    if (size)
        *size = 0;

    // Check cache first
    for (FileCacheEntry *e = sFileCache; e != NULL; e = e->next)
    {
        if (strcmp(e->path, path) == 0)
        {
            if (size)
                *size = e->size;
            return e->data;
        }
    }

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

    FileCacheEntry *entry = malloc(sizeof(*entry));
    if (!entry)
    {
        free(buffer);
        return NULL;
    }

    entry->path = SDL_strdup(path);
    entry->data = buffer;
    entry->size = (size_t)len;
    entry->next = sFileCache;
    sFileCache = entry;

    if (size)
        *size = entry->size;
    return entry->data;
}

void AssetsFreeFile(void *buffer)
{
    (void)buffer;
}

SDL_Surface *AssetsLoadPNG(const char *path)
{
    for (PngCacheEntry *e = sPngCache; e != NULL; e = e->next)
    {
        if (strcmp(e->path, path) == 0)
            return e->surface;
    }

    SDL_Surface *surf = IMG_Load(path);
    if (!surf)
        return NULL;

    SDL_Surface *converted = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(surf);
    if (!converted)
        return NULL;

    PngCacheEntry *entry = malloc(sizeof(*entry));
    if (!entry)
    {
        SDL_FreeSurface(converted);
        return NULL;
    }

    entry->path = SDL_strdup(path);
    entry->surface = converted;
    entry->next = sPngCache;
    sPngCache = entry;

    return converted;
}

u16 *AssetsLoadPal(const char *path, size_t *size)
{
    for (PalCacheEntry *e = sPalCache; e != NULL; e = e->next)
    {
        if (strcmp(e->path, path) == 0)
        {
            if (size)
                *size = e->count * sizeof(u16);
            return e->colors;
        }
    }

    SDL_RWops *rw = SDL_RWFromFile(path, "rb");
    if (!rw)
        return NULL;

    Sint64 len = SDL_RWsize(rw);
    char *text = malloc((size_t)len + 1);
    if (!text)
    {
        SDL_RWclose(rw);
        return NULL;
    }

    if (SDL_RWread(rw, text, 1, (size_t)len) != (size_t)len)
    {
        free(text);
        SDL_RWclose(rw);
        return NULL;
    }
    SDL_RWclose(rw);
    text[len] = '\0';

    char *save;
    char *line = strtok_r(text, "\r\n", &save);
    if (!line || strcmp(line, "JASC-PAL") != 0)
    {
        free(text);
        return NULL;
    }

    line = strtok_r(NULL, "\r\n", &save); // version
    if (!line)
    {
        free(text);
        return NULL;
    }

    line = strtok_r(NULL, "\r\n", &save); // colour count
    if (!line)
    {
        free(text);
        return NULL;
    }

    size_t count = strtoul(line, NULL, 10);
    u16 *colors = malloc(count * sizeof(u16));
    if (!colors)
    {
        free(text);
        return NULL;
    }

    for (size_t i = 0; i < count; i++)
    {
        line = strtok_r(NULL, "\r\n", &save);
        if (!line)
        {
            free(colors);
            free(text);
            return NULL;
        }
        unsigned r, g, b;
        if (sscanf(line, "%u %u %u", &r, &g, &b) != 3)
        {
            free(colors);
            free(text);
            return NULL;
        }
        r >>= 3;
        g >>= 3;
        b >>= 3;
        colors[i] = (r) | (g << 5) | (b << 10);
    }

    free(text);

    PalCacheEntry *entry = malloc(sizeof(*entry));
    if (!entry)
    {
        free(colors);
        return NULL;
    }

    entry->path = SDL_strdup(path);
    entry->colors = colors;
    entry->count = count;
    entry->next = sPalCache;
    sPalCache = entry;

    if (size)
        *size = count * sizeof(u16);
    return colors;
}
