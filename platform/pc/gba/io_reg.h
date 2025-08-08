#ifndef PC_GBA_IO_REG_H
#define PC_GBA_IO_REG_H

#include "types.h"

// Stub hardware registers for desktop build
#define REG_DISPCNT (*(vu16 *)0)
#define REG_WAITCNT (*(vu16 *)0)

// Timers
#define REG_TM1CNT_L (*(vu16 *)0)
#define REG_TM1CNT_H (*(vu16 *)0)

// Interrupt management
#define REG_IME (*(vu16 *)0)
#define INTR_VECTOR (*(void (**)())0)
#define INTR_CHECK (*(vu16 *)0)
#define INTR_FLAG_VBLANK 0x0001
#define INTR_FLAG_HBLANK 0x0002
#define INTR_FLAG_VCOUNT 0x0004
#define INTR_FLAG_SERIAL 0x0080

// DMA register base addresses (unused on PC but required for macros)
#define REG_ADDR_DMA0 0
#define REG_ADDR_DMA1 0
#define REG_ADDR_DMA2 0
#define REG_ADDR_DMA3 0

// DMA control flags (values are irrelevant on PC)
#define DMA_DEST_INC      0
#define DMA_DEST_DEC      0
#define DMA_DEST_FIXED    0
#define DMA_DEST_RELOAD   0
#define DMA_SRC_INC       0
#define DMA_SRC_DEC       0
#define DMA_SRC_FIXED     0
#define DMA_REPEAT        0
#define DMA_16BIT         0
#define DMA_32BIT         0
#define DMA_DREQ_ON       0
#define DMA_START_NOW     0
#define DMA_START_VBLANK  0
#define DMA_START_HBLANK  0
#define DMA_START_SPECIAL 0
#define DMA_START_MASK    0
#define DMA_INTR_ENABLE   0
#define DMA_ENABLE        0

// Wait state control flags (no-op on PC)
#define WAITCNT_PREFETCH_ENABLE 0
#define WAITCNT_WS0_S_1 0
#define WAITCNT_WS0_N_3 0

// Key input constants
#define A_BUTTON        0x0001
#define B_BUTTON        0x0002
#define SELECT_BUTTON   0x0004
#define START_BUTTON    0x0008
#define D_RIGHT         0x0010
#define D_LEFT          0x0020
#define D_UP            0x0040
#define D_DOWN          0x0080
#define R_BUTTON        0x0100
#define L_BUTTON        0x0200
#define DPAD_RIGHT      D_RIGHT
#define DPAD_LEFT       D_LEFT
#define DPAD_UP         D_UP
#define DPAD_DOWN       D_DOWN
#define DPAD_ANY        (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN)

#endif // PC_GBA_IO_REG_H
