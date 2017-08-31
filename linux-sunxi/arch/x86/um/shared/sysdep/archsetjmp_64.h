/*
 * arch/x86/um/shared/sysdep/archsetjmp_64.h
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
 * arch/um/include/sysdep-x86_64/archsetjmp.h
 */

#ifndef _KLIBC_ARCHSETJMP_H
#define _KLIBC_ARCHSETJMP_H

struct __jmp_buf {
	unsigned long __rbx;
	unsigned long __rsp;
	unsigned long __rbp;
	unsigned long __r12;
	unsigned long __r13;
	unsigned long __r14;
	unsigned long __r15;
	unsigned long __rip;
};

typedef struct __jmp_buf jmp_buf[1];

#define JB_IP __rip
#define JB_SP __rsp

#endif				/* _SETJMP_H */
