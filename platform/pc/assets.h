#ifndef PLATFORM_PC_ASSETS_H
#define PLATFORM_PC_ASSETS_H

#include <stddef.h>

void *AssetsLoadFile(const char *path, size_t *size);
void AssetsFreeFile(void *buffer);

#endif // PLATFORM_PC_ASSETS_H
