/*
 * arch/mips/include/asm/mach-malta/mach-gt64120.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *  This is a direct copy of the ev96100.h file, with a global
 * search and replace.  The numbers are the same.
 *
 *  The reason I'm duplicating this is so that the 64120/96100
 * defines won't be confusing in the source code.
 */
#ifndef _ASM_MACH_MIPS_MACH_GT64120_DEP_H
#define _ASM_MACH_MIPS_MACH_GT64120_DEP_H

#define MIPS_GT_BASE	0x1be00000

extern unsigned long _pcictrl_gt64120;
/*
 *   GT64120 config space base address
 */
#define GT64120_BASE	_pcictrl_gt64120

#endif /* _ASM_MACH_MIPS_MACH_GT64120_DEP_H */
