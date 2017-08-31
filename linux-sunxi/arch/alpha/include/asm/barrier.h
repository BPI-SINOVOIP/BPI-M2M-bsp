/*
 * arch/alpha/include/asm/barrier.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __BARRIER_H
#define __BARRIER_H

#include <asm/compiler.h>

#define mb() \
__asm__ __volatile__("mb": : :"memory")

#define rmb() \
__asm__ __volatile__("mb": : :"memory")

#define wmb() \
__asm__ __volatile__("wmb": : :"memory")

#define read_barrier_depends() \
__asm__ __volatile__("mb": : :"memory")

#ifdef CONFIG_SMP
#define __ASM_SMP_MB	"\tmb\n"
#define smp_mb()	mb()
#define smp_rmb()	rmb()
#define smp_wmb()	wmb()
#define smp_read_barrier_depends()	read_barrier_depends()
#else
#define __ASM_SMP_MB
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#define smp_read_barrier_depends()	do { } while (0)
#endif

#define set_mb(var, value) \
do { var = value; mb(); } while (0)

#endif		/* __BARRIER_H */
