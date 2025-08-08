#ifndef PC_GBA_GBA_H
#define PC_GBA_GBA_H

#include "types.h"
#include "defines.h"
#include "io_reg.h"
#include "syscall.h"

// Provide minimal CPU/DMA helper macros so existing code that expects
// the GBA's macro.h can compile without pulling in hardware-specific
// implementations that trigger pointer truncation warnings on 64-bit
// hosts.
#include <string.h>

static inline void PC_DmaCopy(const void *src, void *dest, size_t size)
{
    memcpy(dest, src, size);
}

static inline void PC_DmaFill16(u16 value, void *dest, size_t size)
{
    u16 *d = dest;
    for (size_t i = 0; i < size / 2; i++)
        d[i] = value;
}

static inline void PC_DmaFill32(u32 value, void *dest, size_t size)
{
    u32 *d = dest;
    for (size_t i = 0; i < size / 4; i++)
        d[i] = value;
}

#define DmaCopy16(num, src, dest, size) PC_DmaCopy((src), (dest), (size))
#define DmaCopy32(num, src, dest, size) PC_DmaCopy((src), (dest), (size))
#define DmaFill16(num, value, dest, size) PC_DmaFill16((value), (dest), (size))
#define DmaFill32(num, value, dest, size) PC_DmaFill32((value), (dest), (size))
#define DmaStop(num) ((void)0)

#define CpuFill16(value, dest, size) PC_DmaFill16((value), (dest), (size))
#define CpuFill32(value, dest, size) PC_DmaFill32((value), (dest), (size))
#define CpuCopy16(src, dest, size) PC_DmaCopy((src), (dest), (size))
#define CpuCopy32(src, dest, size) PC_DmaCopy((src), (dest), (size))

#endif // PC_GBA_GBA_H
