/*
 * arch/microblaze/include/asm/ftrace.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_MICROBLAZE_FTRACE
#define _ASM_MICROBLAZE_FTRACE

#ifdef CONFIG_FUNCTION_TRACER

#define MCOUNT_ADDR		((long)(_mcount))
#define MCOUNT_INSN_SIZE	8 /* sizeof mcount call */

#ifndef __ASSEMBLY__
extern void _mcount(void);
extern void ftrace_call_graph(void);
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
/* reloction of mcount call site is the same as the address */
static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	return addr;
}

struct dyn_arch_ftrace {
};
#endif /* CONFIG_DYNAMIC_FTRACE */

#endif /* CONFIG_FUNCTION_TRACER */
#endif /* _ASM_MICROBLAZE_FTRACE */
