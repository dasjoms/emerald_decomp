#ifndef GUARD_GBA_GBA_H
#define GUARD_GBA_GBA_H

#ifdef PLATFORM_PC
#include "../../platform/pc/gba/gba.h"
#else
#include "gba/defines.h"
#include "gba/io_reg.h"
#include "gba/types.h"
#include "gba/multiboot.h"
#include "gba/syscall.h"
#include "gba/macro.h"
#include "gba/isagbprint.h"
#endif

#endif // GUARD_GBA_GBA_H
