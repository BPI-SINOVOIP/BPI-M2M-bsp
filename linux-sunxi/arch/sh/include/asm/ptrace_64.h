/*
 * arch/sh/include/asm/ptrace_64.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_PTRACE_64_H
#define __ASM_SH_PTRACE_64_H

struct pt_regs {
	unsigned long long pc;
	unsigned long long sr;
	long long syscall_nr;
	unsigned long long regs[63];
	unsigned long long tregs[8];
	unsigned long long pad[2];
};

#ifdef __KERNEL__

#define MAX_REG_OFFSET		offsetof(struct pt_regs, tregs[7])
static inline long regs_return_value(struct pt_regs *regs)
{
	return regs->regs[3];
}

#endif /* __KERNEL__ */

#endif /* __ASM_SH_PTRACE_64_H */
