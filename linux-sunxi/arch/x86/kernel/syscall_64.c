/*
 * arch/x86/kernel/syscall_64.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* System call table for x86-64. */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/asm-offsets.h>

#define __SYSCALL_COMMON(nr, sym, compat) __SYSCALL_64(nr, sym, compat)

#ifdef CONFIG_X86_X32_ABI
# define __SYSCALL_X32(nr, sym, compat) __SYSCALL_64(nr, sym, compat)
#else
# define __SYSCALL_X32(nr, sym, compat) /* nothing */
#endif

#define __SYSCALL_64(nr, sym, compat) extern asmlinkage void sym(void) ;
#include <asm/syscalls_64.h>
#undef __SYSCALL_64

#define __SYSCALL_64(nr, sym, compat) [nr] = sym,

typedef void (*sys_call_ptr_t)(void);

extern void sys_ni_syscall(void);

const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
#include <asm/syscalls_64.h>
};
