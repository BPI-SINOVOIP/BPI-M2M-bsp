/*
 * arch/x86/ia32/syscall_ia32.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* System call table for ia32 emulation. */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/asm-offsets.h>

#define __SYSCALL_I386(nr, sym, compat) extern asmlinkage void compat(void) ;
#include <asm/syscalls_32.h>
#undef __SYSCALL_I386

#define __SYSCALL_I386(nr, sym, compat) [nr] = compat,

typedef void (*sys_call_ptr_t)(void);

extern void compat_ni_syscall(void);

const sys_call_ptr_t ia32_sys_call_table[__NR_ia32_syscall_max+1] = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_ia32_syscall_max] = &compat_ni_syscall,
#include <asm/syscalls_32.h>
};
