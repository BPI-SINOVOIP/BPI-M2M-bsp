/*
 * arch/cris/include/asm/ptrace.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CRIS_PTRACE_H
#define _CRIS_PTRACE_H

#include <arch/ptrace.h>

#ifdef __KERNEL__

/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13

#define profile_pc(regs) instruction_pointer(regs)

#endif /* __KERNEL__ */

#endif /* _CRIS_PTRACE_H */
