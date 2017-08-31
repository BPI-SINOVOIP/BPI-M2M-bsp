/*
 * arch/x86/include/asm/kmemcheck.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef ASM_X86_KMEMCHECK_H
#define ASM_X86_KMEMCHECK_H

#include <linux/types.h>
#include <asm/ptrace.h>

#ifdef CONFIG_KMEMCHECK
bool kmemcheck_active(struct pt_regs *regs);

void kmemcheck_show(struct pt_regs *regs);
void kmemcheck_hide(struct pt_regs *regs);

bool kmemcheck_fault(struct pt_regs *regs,
	unsigned long address, unsigned long error_code);
bool kmemcheck_trap(struct pt_regs *regs);
#else
static inline bool kmemcheck_active(struct pt_regs *regs)
{
	return false;
}

static inline void kmemcheck_show(struct pt_regs *regs)
{
}

static inline void kmemcheck_hide(struct pt_regs *regs)
{
}

static inline bool kmemcheck_fault(struct pt_regs *regs,
	unsigned long address, unsigned long error_code)
{
	return false;
}

static inline bool kmemcheck_trap(struct pt_regs *regs)
{
	return false;
}
#endif /* CONFIG_KMEMCHECK */

#endif
