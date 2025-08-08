#include "platform.h"
#include "gba/syscall.h"
#include <string.h>

void IntrWait(u8 clear, u16 flags)
{
    (void)clear;
    (void)flags;
    PlatformWaitForVBlank();
}

void VBlankIntrWait(void)
{
    PlatformWaitForVBlank();
}

void RegisterRamReset(u32 resetFlags)
{
    (void)resetFlags;
}

void CpuSet(const void *src, void *dest, u32 control)
{
    u32 count = control & 0x1FFFFF;
    if (control & CPU_SET_32BIT)
    {
        u32 *d = dest;
        if (control & CPU_SET_SRC_FIXED)
        {
            u32 value = *(const u32 *)src;
            for (u32 i = 0; i < count; i++)
                d[i] = value;
        }
        else
        {
            const u32 *s = src;
            for (u32 i = 0; i < count; i++)
                d[i] = s[i];
        }
    }
    else
    {
        u16 *d = dest;
        if (control & CPU_SET_SRC_FIXED)
        {
            u16 value = *(const u16 *)src;
            for (u32 i = 0; i < count; i++)
                d[i] = value;
        }
        else
        {
            const u16 *s = src;
            for (u32 i = 0; i < count; i++)
                d[i] = s[i];
        }
    }
}

void CpuFastSet(const void *src, void *dest, u32 control)
{
    u32 count = control & 0x1FFFFF;
    u32 *d = dest;
    if (control & CPU_FAST_SET_SRC_FIXED)
    {
        u32 value = *(const u32 *)src;
        for (u32 i = 0; i < count; i++)
            d[i] = value;
    }
    else
    {
        const u32 *s = src;
        for (u32 i = 0; i < count; i++)
            d[i] = s[i];
    }
}

static void LZ77Decompress(const u8 *src, u8 *dest)
{
    if (src[0] != 0x10)
        return;

    u32 decompressedSize = src[1] | (src[2] << 8) | (src[3] << 16);
    src += 4;
    u32 out = 0;

    while (out < decompressedSize)
    {
        u8 flags = *src++;
        for (int i = 0; i < 8 && out < decompressedSize; i++)
        {
            if (!(flags & 0x80))
            {
                dest[out++] = *src++;
            }
            else
            {
                u8 b1 = *src++;
                u8 b2 = *src++;
                u32 disp = ((b1 & 0xF) << 8) | b2;
                u32 len = (b1 >> 4) + 3;
                disp += 1;
                for (u32 j = 0; j < len; j++)
                {
                    dest[out] = dest[out - disp];
                    out++;
                }
            }
            flags <<= 1;
        }
    }
}

void LZ77UnCompWram(const u32 *src, void *dest)
{
    LZ77Decompress((const u8 *)src, dest);
}

void LZ77UnCompVram(const u32 *src, void *dest)
{
    LZ77Decompress((const u8 *)src, dest);
}

