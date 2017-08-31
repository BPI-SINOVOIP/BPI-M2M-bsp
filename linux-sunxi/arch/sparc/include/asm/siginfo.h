/*
 * arch/sparc/include/asm/siginfo.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SPARC_SIGINFO_H
#define __SPARC_SIGINFO_H

#if defined(__sparc__) && defined(__arch64__)

#define SI_PAD_SIZE32	((SI_MAX_SIZE/sizeof(int)) - 3)
#define __ARCH_SI_PREAMBLE_SIZE	(4 * sizeof(int))
#define __ARCH_SI_BAND_T int

#endif /* defined(__sparc__) && defined(__arch64__) */


#define __ARCH_SI_TRAPNO

#include <asm-generic/siginfo.h>

#ifdef __KERNEL__

#ifdef CONFIG_COMPAT

struct compat_siginfo;

#endif /* CONFIG_COMPAT */

#endif /* __KERNEL__ */

#define SI_NOINFO	32767		/* no information in siginfo_t */

/*
 * SIGEMT si_codes
 */
#define EMT_TAGOVF	(__SI_FAULT|1)	/* tag overflow */
#define NSIGEMT		1

#endif /* !(__SPARC_SIGINFO_H) */
