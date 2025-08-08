#ifndef PC_GBA_DEFINES_H
#define PC_GBA_DEFINES_H

#define TRUE 1
#define FALSE 0

#define IWRAM_DATA
#define EWRAM_DATA
#define COMMON_DATA
#define UNUSED __attribute__((unused))

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#define ALIGNED(n) __attribute__((aligned(n)))

struct SoundInfo;
extern struct SoundInfo *gSoundInfoPtr;
#define SOUND_INFO_PTR gSoundInfoPtr

// Basic VRAM/Palette constants used by core code. Values are placeholders
// sufficient for desktop builds and do not map to real hardware addresses.
#define PLTT          0
#define BG_PLTT       PLTT
#define BG_PLTT_SIZE  0x200
#define OBJ_PLTT      (PLTT + BG_PLTT_SIZE)
#define OBJ_PLTT_SIZE 0x200
#define PLTT_SIZE     (BG_PLTT_SIZE + OBJ_PLTT_SIZE)
#define PLTT_SIZEOF(n) ((n) * sizeof(u16))

#define TILE_SIZE(n) ((n) * 8)
#define TILE_SIZE_4BPP TILE_SIZE(4)
#define TILE_OFFSET_4BPP(n) ((n) * TILE_SIZE_4BPP)
#define PLTT_SIZE_4BPP PLTT_SIZEOF(16)
#define PLTT_OFFSET_4BPP(n) ((n) * PLTT_SIZE_4BPP)

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 160

#endif // PC_GBA_DEFINES_H
