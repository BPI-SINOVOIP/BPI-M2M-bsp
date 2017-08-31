/*
 * arch/x86/include/asm/seccomp_64.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_SECCOMP_64_H
#define _ASM_X86_SECCOMP_64_H

#include <linux/unistd.h>
#include <asm/ia32_unistd.h>

#define __NR_seccomp_read __NR_read
#define __NR_seccomp_write __NR_write
#define __NR_seccomp_exit __NR_exit
#define __NR_seccomp_sigreturn __NR_rt_sigreturn

#define __NR_seccomp_read_32 __NR_ia32_read
#define __NR_seccomp_write_32 __NR_ia32_write
#define __NR_seccomp_exit_32 __NR_ia32_exit
#define __NR_seccomp_sigreturn_32 __NR_ia32_sigreturn

#endif /* _ASM_X86_SECCOMP_64_H */
