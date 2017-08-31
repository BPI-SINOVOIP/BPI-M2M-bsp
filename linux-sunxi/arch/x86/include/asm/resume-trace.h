/*
 * arch/x86/include/asm/resume-trace.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_X86_RESUME_TRACE_H
#define _ASM_X86_RESUME_TRACE_H

#include <asm/asm.h>

#define TRACE_RESUME(user)					\
do {								\
	if (pm_trace_enabled) {					\
		const void *tracedata;				\
		asm volatile(_ASM_MOV " $1f,%0\n"		\
			     ".section .tracedata,\"a\"\n"	\
			     "1:\t.word %c1\n\t"		\
			     _ASM_PTR " %c2\n"			\
			     ".previous"			\
			     :"=r" (tracedata)			\
			     : "i" (__LINE__), "i" (__FILE__));	\
		generate_resume_trace(tracedata, user);		\
	}							\
} while (0)

#endif /* _ASM_X86_RESUME_TRACE_H */
