#include "global.h"
#include "fs.h"
#include "save.h"
#include "agb_flash.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

static const char *GetSaveFilePath(void)
{
    static char sPath[256];
    const char *dir = FsGetSavePath();
    snprintf(sPath, sizeof(sPath), "%s/slot0.sav", dir);
    return sPath;
}

static void EnsureSaveFile(void)
{
    const char *dir = FsGetSavePath();
#ifdef _WIN32
    _mkdir(dir);
#else
    mkdir(dir, 0755);
#endif
    const char *path = GetSaveFilePath();
    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        f = fopen(path, "wb");
        if (f != NULL)
        {
            u8 blank[SECTOR_SIZE];
            memset(blank, 0xFF, sizeof(blank));
            for (int i = 0; i < SECTORS_COUNT; i++)
                fwrite(blank, 1, sizeof(blank), f);
            fclose(f);
        }
    }
    else
    {
        fclose(f);
    }
}

u16 IdentifyFlash(void)
{
    EnsureSaveFile();
    return 0;
}

u16 SetFlashTimerIntr(u8 timerNum, void (**intrFunc)(void))
{
    (void)timerNum;
    (void)intrFunc;
    return 0;
}

void ReadFlash(u16 sectorNum, u32 offset, u8 *dest, u32 size)
{
    EnsureSaveFile();
    FILE *f = fopen(GetSaveFilePath(), "rb");
    if (f == NULL)
    {
        memset(dest, 0xFF, size);
        return;
    }
    fseek(f, sectorNum * SECTOR_SIZE + offset, SEEK_SET);
    size_t readBytes = fread(dest, 1, size, f);
    (void)readBytes;
    fclose(f);
}

u32 ProgramFlashSectorAndVerify(u16 sectorNum, u8 *src)
{
    EnsureSaveFile();
    FILE *f = fopen(GetSaveFilePath(), "rb+");
    if (f == NULL)
        return 1;
    fseek(f, sectorNum * SECTOR_SIZE, SEEK_SET);
    size_t written = fwrite(src, 1, SECTOR_SIZE, f);
    fflush(f);
    fclose(f);
    return written == SECTOR_SIZE ? 0 : 1;
}

static u16 PcEraseFlashSector(u16 sectorNum)
{
    EnsureSaveFile();
    FILE *f = fopen(GetSaveFilePath(), "rb+");
    if (f == NULL)
        return 1;
    u8 blank[SECTOR_SIZE];
    memset(blank, 0xFF, sizeof(blank));
    fseek(f, sectorNum * SECTOR_SIZE, SEEK_SET);
    fwrite(blank, 1, sizeof(blank), f);
    fflush(f);
    fclose(f);
    return 0;
}

u16 (*EraseFlashSector)(u16) = PcEraseFlashSector;

