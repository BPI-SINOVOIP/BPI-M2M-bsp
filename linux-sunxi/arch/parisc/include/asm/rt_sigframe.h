/*
 * arch/parisc/include/asm/rt_sigframe.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_PARISC_RT_SIGFRAME_H
#define _ASM_PARISC_RT_SIGFRAME_H

#define SIGRETURN_TRAMP 4
#define SIGRESTARTBLOCK_TRAMP 5 
#define TRAMP_SIZE (SIGRETURN_TRAMP + SIGRESTARTBLOCK_TRAMP)

struct rt_sigframe {
	/* XXX: Must match trampoline size in arch/parisc/kernel/signal.c 
	        Secondary to that it must protect the ERESTART_RESTARTBLOCK
		trampoline we left on the stack (we were bad and didn't 
		change sp so we could run really fast.) */
	unsigned int tramp[TRAMP_SIZE];
	struct siginfo info;
	struct ucontext uc;
};

#define	SIGFRAME		128
#define FUNCTIONCALLFRAME	96
#define PARISC_RT_SIGFRAME_SIZE					\
	(((sizeof(struct rt_sigframe) + FUNCTIONCALLFRAME) + SIGFRAME) & -SIGFRAME)

#endif
