/*
 * arch/x86/include/asm/ia32_unistd.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_IA32_UNISTD_H
#define _ASM_X86_IA32_UNISTD_H

/*
 * This file contains the system call numbers of the ia32 compat ABI,
 * this is for the kernel only.
 */
#define __SYSCALL_ia32_NR(x) (x)
#include <asm/unistd_32_ia32.h>

#endif /* _ASM_X86_IA32_UNISTD_H */
