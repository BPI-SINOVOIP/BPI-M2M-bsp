/*
 * arch/sparc/include/asm/barrier_32.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SPARC_BARRIER_H
#define __SPARC_BARRIER_H

/* XXX Change this if we ever use a PSO mode kernel. */
#define mb()	__asm__ __volatile__ ("" : : : "memory")
#define rmb()	mb()
#define wmb()	mb()
#define read_barrier_depends()	do { } while(0)
#define set_mb(__var, __value)  do { __var = __value; mb(); } while(0)
#define smp_mb()	__asm__ __volatile__("":::"memory")
#define smp_rmb()	__asm__ __volatile__("":::"memory")
#define smp_wmb()	__asm__ __volatile__("":::"memory")
#define smp_read_barrier_depends()	do { } while(0)

#endif /* !(__SPARC_BARRIER_H) */
