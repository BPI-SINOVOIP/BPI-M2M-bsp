/*
 * arch/x86/include/asm/sighandling.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_SIGHANDLING_H
#define _ASM_X86_SIGHANDLING_H

#include <linux/compiler.h>
#include <linux/ptrace.h>
#include <linux/signal.h>

#include <asm/processor-flags.h>

#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

#define __FIX_EFLAGS	(X86_EFLAGS_AC | X86_EFLAGS_OF | \
			 X86_EFLAGS_DF | X86_EFLAGS_TF | X86_EFLAGS_SF | \
			 X86_EFLAGS_ZF | X86_EFLAGS_AF | X86_EFLAGS_PF | \
			 X86_EFLAGS_CF)

void signal_fault(struct pt_regs *regs, void __user *frame, char *where);

int restore_sigcontext(struct pt_regs *regs, struct sigcontext __user *sc,
		       unsigned long *pax);
int setup_sigcontext(struct sigcontext __user *sc, void __user *fpstate,
		     struct pt_regs *regs, unsigned long mask);

#endif /* _ASM_X86_SIGHANDLING_H */
