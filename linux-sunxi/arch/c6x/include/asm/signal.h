/*
 * arch/c6x/include/asm/signal.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_C6X_SIGNAL_H
#define _ASM_C6X_SIGNAL_H

#include <asm-generic/signal.h>

#ifndef __ASSEMBLY__
#include <linux/linkage.h>

struct pt_regs;

extern asmlinkage int do_rt_sigreturn(struct pt_regs *regs);
extern asmlinkage void do_notify_resume(struct pt_regs *regs,
					u32 thread_info_flags,
					int syscall);
#endif

#endif /* _ASM_C6X_SIGNAL_H */
