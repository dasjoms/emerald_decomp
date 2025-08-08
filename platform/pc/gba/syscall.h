#ifndef PC_GBA_SYSCALL_H
#define PC_GBA_SYSCALL_H

// Pull in declarations for BIOS functions so that code referencing
// them will compile. The actual implementations will be provided by
// the PC platform layer.

// Forward declaration to satisfy the prototype in the GBA headers.
// The real structure is unused on PC, but providing this declaration
// silences warnings about an incomplete type when including the
// original syscall declarations.
struct MultiBootParam;

#include "../../../include/gba/syscall.h"

#endif // PC_GBA_SYSCALL_H

