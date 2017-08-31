/*
 * arch/alpha/include/asm/special_insns.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ALPHA_SPECIAL_INSNS_H
#define __ALPHA_SPECIAL_INSNS_H

enum implver_enum {
	IMPLVER_EV4,
	IMPLVER_EV5,
	IMPLVER_EV6
};

#ifdef CONFIG_ALPHA_GENERIC
#define implver()				\
({ unsigned long __implver;			\
   __asm__ ("implver %0" : "=r"(__implver));	\
   (enum implver_enum) __implver; })
#else
/* Try to eliminate some dead code.  */
#ifdef CONFIG_ALPHA_EV4
#define implver() IMPLVER_EV4
#endif
#ifdef CONFIG_ALPHA_EV5
#define implver() IMPLVER_EV5
#endif
#if defined(CONFIG_ALPHA_EV6)
#define implver() IMPLVER_EV6
#endif
#endif

enum amask_enum {
	AMASK_BWX = (1UL << 0),
	AMASK_FIX = (1UL << 1),
	AMASK_CIX = (1UL << 2),
	AMASK_MAX = (1UL << 8),
	AMASK_PRECISE_TRAP = (1UL << 9),
};

#define amask(mask)						\
({ unsigned long __amask, __input = (mask);			\
   __asm__ ("amask %1,%0" : "=r"(__amask) : "rI"(__input));	\
   __amask; })

#endif /* __ALPHA_SPECIAL_INSNS_H */
