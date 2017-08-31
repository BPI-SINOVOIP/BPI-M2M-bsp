/*
 * arch/score/include/asm/syscalls.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SCORE_SYSCALLS_H
#define _ASM_SCORE_SYSCALLS_H

asmlinkage long score_clone(struct pt_regs *regs);
asmlinkage long score_execve(struct pt_regs *regs);
asmlinkage long score_sigaltstack(struct pt_regs *regs);
asmlinkage long score_rt_sigreturn(struct pt_regs *regs);

#include <asm-generic/syscalls.h>

#endif /* _ASM_SCORE_SYSCALLS_H */
