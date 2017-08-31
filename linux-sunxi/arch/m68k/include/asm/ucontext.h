/*
 * arch/m68k/include/asm/ucontext.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _M68K_UCONTEXT_H
#define _M68K_UCONTEXT_H

typedef int greg_t;
#define NGREG 18
typedef greg_t gregset_t[NGREG];

typedef struct fpregset {
	int f_fpcntl[3];
	int f_fpregs[8*3];
} fpregset_t;

struct mcontext {
	int version;
	gregset_t gregs;
	fpregset_t fpregs;
};

#define MCONTEXT_VERSION 2

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	struct mcontext	  uc_mcontext;
	unsigned long	  uc_filler[80];
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
};

#endif
