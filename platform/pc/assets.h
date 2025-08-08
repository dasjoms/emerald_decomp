#ifndef PLATFORM_PC_ASSETS_H
#define PLATFORM_PC_ASSETS_H

#include <stddef.h>
#include <SDL.h>
#include "gba/types.h"

// Returns a pointer to the file contents. Subsequent calls with the same path
// reuse a cached copy to avoid redundant disk access. The returned buffer
// persists for the lifetime of the program and should not be freed directly.
void *AssetsLoadFile(const char *path, size_t *size);

// Convenience wrapper that decodes a PNG file into an ARGB8888 surface. The
// surface is cached by path and is owned by the asset system.
SDL_Surface *AssetsLoadPNG(const char *path);

// Parse a text .pal file (JASC-PAL format) into an array of RGB555 colours.
// The returned buffer is cached and owned by the asset system.
u16 *AssetsLoadPal(const char *path, size_t *size);

// Load a paletted PNG and convert it to 4bpp tile data using the given
// palette. The returned buffer is cached and owned by the asset system.
u8 *AssetsLoad4bpp(const char *pngPath, const char *palPath, size_t *size);

// No-op placeholder provided for API compatibility with earlier loaders. The
// cache retains loaded resources for the program lifetime.
void AssetsFreeFile(void *buffer);

#endif // PLATFORM_PC_ASSETS_H
