/*
 * arch/sh/include/asm/traps.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_TRAPS_H
#define __ASM_SH_TRAPS_H

#include <linux/compiler.h>

#ifdef CONFIG_SUPERH32
# include "traps_32.h"
#else
# include "traps_64.h"
#endif

BUILD_TRAP_HANDLER(address_error);
BUILD_TRAP_HANDLER(debug);
BUILD_TRAP_HANDLER(bug);
BUILD_TRAP_HANDLER(breakpoint);
BUILD_TRAP_HANDLER(singlestep);
BUILD_TRAP_HANDLER(fpu_error);
BUILD_TRAP_HANDLER(fpu_state_restore);
BUILD_TRAP_HANDLER(nmi);

#endif /* __ASM_SH_TRAPS_H */
