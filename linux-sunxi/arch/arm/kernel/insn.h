/*
 * arch/arm/kernel/insn.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARM_INSN_H
#define __ASM_ARM_INSN_H

static inline unsigned long
arm_gen_nop(void)
{
#ifdef CONFIG_THUMB2_KERNEL
	return 0xf3af8000; /* nop.w */
#else
	return 0xe1a00000; /* mov r0, r0 */
#endif
}

unsigned long
__arm_gen_branch(unsigned long pc, unsigned long addr, bool link);

static inline unsigned long
arm_gen_branch(unsigned long pc, unsigned long addr)
{
	return __arm_gen_branch(pc, addr, false);
}

static inline unsigned long
arm_gen_branch_link(unsigned long pc, unsigned long addr)
{
	return __arm_gen_branch(pc, addr, true);
}

#endif
