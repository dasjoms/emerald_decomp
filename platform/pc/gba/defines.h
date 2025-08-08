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

#endif // PC_GBA_DEFINES_H
