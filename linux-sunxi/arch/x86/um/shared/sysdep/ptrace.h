/*
 * arch/x86/um/shared/sysdep/ptrace.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SYSDEP_X86_PTRACE_H
#define __SYSDEP_X86_PTRACE_H

#ifdef __i386__
#include "ptrace_32.h"
#else
#include "ptrace_64.h"
#endif

static inline long regs_return_value(struct uml_pt_regs *regs)
{
	return UPT_SYSCALL_RET(regs);
}

#endif /* __SYSDEP_X86_PTRACE_H */
