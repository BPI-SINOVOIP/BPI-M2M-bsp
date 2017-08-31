/*
 * arch/mips/include/asm/bug.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_BUG_H
#define __ASM_BUG_H

#include <linux/compiler.h>
#include <asm/sgidefs.h>

#ifdef CONFIG_BUG

#include <asm/break.h>

static inline void __noreturn BUG(void)
{
	__asm__ __volatile__("break %0" : : "i" (BRK_BUG));
	unreachable();
}

#define HAVE_ARCH_BUG

#if (_MIPS_ISA > _MIPS_ISA_MIPS1)

static inline void  __BUG_ON(unsigned long condition)
{
	if (__builtin_constant_p(condition)) {
		if (condition)
			BUG();
		else
			return;
	}
	__asm__ __volatile__("tne $0, %0, %1"
			     : : "r" (condition), "i" (BRK_BUG));
}

#define BUG_ON(C) __BUG_ON((unsigned long)(C))

#define HAVE_ARCH_BUG_ON

#endif /* _MIPS_ISA > _MIPS_ISA_MIPS1 */

#endif

#include <asm-generic/bug.h>

#endif /* __ASM_BUG_H */
