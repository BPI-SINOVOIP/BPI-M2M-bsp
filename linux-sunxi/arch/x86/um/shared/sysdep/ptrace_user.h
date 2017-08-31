/*
 * arch/x86/um/shared/sysdep/ptrace_user.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <generated/user_constants.h>

#define PT_OFFSET(r) ((r) * sizeof(long))

#define PT_SYSCALL_NR(regs) ((regs)[HOST_ORIG_AX])
#define PT_SYSCALL_NR_OFFSET PT_OFFSET(HOST_ORIG_AX)

#define PT_SYSCALL_RET_OFFSET PT_OFFSET(HOST_AX)

#define REGS_IP_INDEX HOST_IP
#define REGS_SP_INDEX HOST_SP

#ifdef __i386__
#define FP_SIZE ((HOST_FPX_SIZE > HOST_FP_SIZE) ? HOST_FPX_SIZE : HOST_FP_SIZE)
#else
#define FP_SIZE HOST_FP_SIZE

/*
 * x86_64 FC3 doesn't define this in /usr/include/linux/ptrace.h even though
 * it's defined in the kernel's include/linux/ptrace.h. Additionally, use the
 * 2.4 name and value for 2.4 host compatibility.
 */
#ifndef PTRACE_OLDSETOPTIONS
#define PTRACE_OLDSETOPTIONS 21
#endif

#endif
