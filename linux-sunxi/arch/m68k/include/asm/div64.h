/*
 * arch/m68k/include/asm/div64.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _M68K_DIV64_H
#define _M68K_DIV64_H

#ifdef CONFIG_CPU_HAS_NO_MULDIV64
#include <asm-generic/div64.h>
#else

#include <linux/types.h>

/* n = n / base; return rem; */

#define do_div(n, base) ({					\
	union {							\
		unsigned long n32[2];				\
		unsigned long long n64;				\
	} __n;							\
	unsigned long __rem, __upper;				\
								\
	__n.n64 = (n);						\
	if ((__upper = __n.n32[0])) {				\
		asm ("divul.l %2,%1:%0"				\
			: "=d" (__n.n32[0]), "=d" (__upper)	\
			: "d" (base), "0" (__n.n32[0]));	\
	}							\
	asm ("divu.l %2,%1:%0"					\
		: "=d" (__n.n32[1]), "=d" (__rem)		\
		: "d" (base), "1" (__upper), "0" (__n.n32[1]));	\
	(n) = __n.n64;						\
	__rem;							\
})

#endif /* CONFIG_CPU_HAS_NO_MULDIV64 */

#endif /* _M68K_DIV64_H */
