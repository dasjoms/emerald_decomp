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

typedef struct TilesCacheEntry
{
    char *key;
    u8 *tiles;
    size_t size;
    struct TilesCacheEntry *next;
} TilesCacheEntry;

static FileCacheEntry *sFileCache;
static PngCacheEntry *sPngCache;
static PalCacheEntry *sPalCache;
static TilesCacheEntry *sTilesCache;

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

u8 *AssetsLoad4bpp(const char *pngPath, const char *palPath, size_t *size)
{
    if (size)
        *size = 0;

    char *key;
    if (SDL_asprintf(&key, "%s|%s", pngPath, palPath ? palPath : "") < 0)
        return NULL;

    for (TilesCacheEntry *e = sTilesCache; e != NULL; e = e->next)
    {
        if (strcmp(e->key, key) == 0)
        {
            if (size)
                *size = e->size;
            SDL_free(key);
            return e->tiles;
        }
    }

    SDL_Surface *surf = AssetsLoadPNG(pngPath);
    size_t palSize = 0;
    u16 *pal = palPath ? AssetsLoadPal(palPath, &palSize) : NULL;
    if (!surf || !pal)
    {
        SDL_free(key);
        return NULL;
    }

    int width = surf->w;
    int height = surf->h;
    if (width % 8 != 0 || height % 8 != 0)
    {
        SDL_free(key);
        return NULL;
    }

    size_t tiles = (width * height) / (8 * 8);
    size_t bufSize = tiles * 32; // 32 bytes per tile
    u8 *buffer = malloc(bufSize);
    if (!buffer)
    {
        SDL_free(key);
        return NULL;
    }

    u32 *pixels = surf->pixels;
    int pitch = surf->pitch / 4; // in u32 units
    size_t palCount = palSize / sizeof(u16);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            u32 pixel = pixels[y * pitch + x];
            u8 r, g, b, a;
            SDL_GetRGBA(pixel, surf->format, &r, &g, &b, &a);
            u16 color = (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10);
            u8 index = 0;
            for (; index < palCount; index++)
            {
                if (pal[index] == color)
                    break;
            }
            size_t pixelIndex = y * width + x;
            size_t byteIndex = pixelIndex / 2;
            if (pixelIndex & 1)
                buffer[byteIndex] |= index << 4;
            else
                buffer[byteIndex] = index & 0xF;
        }
    }

    TilesCacheEntry *entry = malloc(sizeof(*entry));
    if (!entry)
    {
        free(buffer);
        SDL_free(key);
        return NULL;
    }

    entry->key = key;
    entry->tiles = buffer;
    entry->size = bufSize;
    entry->next = sTilesCache;
    sTilesCache = entry;

    if (size)
        *size = bufSize;
    return buffer;
}
