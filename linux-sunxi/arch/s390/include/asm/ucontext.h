/*
 * arch/s390/include/asm/ucontext.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *  include/asm-s390/ucontext.h
 *
 *  S390 version
 *
 *  Derived from "include/asm-i386/ucontext.h"
 */

#ifndef _ASM_S390_UCONTEXT_H
#define _ASM_S390_UCONTEXT_H

#define UC_EXTENDED	0x00000001

#ifndef __s390x__

struct ucontext_extended {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	_sigregs	  uc_mcontext;
	unsigned long	  uc_sigmask[2];
	unsigned long	  uc_gprs_high[16];
};

#endif

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	_sigregs          uc_mcontext;
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
};

#endif /* !_ASM_S390_UCONTEXT_H */
