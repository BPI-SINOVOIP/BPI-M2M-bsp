/*
 * arch/um/include/asm/smp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_SMP_H
#define __UM_SMP_H

#ifdef CONFIG_SMP

#include "linux/bitops.h"
#include "asm/current.h"
#include "linux/cpumask.h"

#define raw_smp_processor_id() (current_thread->cpu)

#define cpu_logical_map(n) (n)
#define cpu_number_map(n) (n)
extern int hard_smp_processor_id(void);
#define NO_PROC_ID -1

extern int ncpus;


static inline void smp_cpus_done(unsigned int maxcpus)
{
}

extern struct task_struct *idle_threads[NR_CPUS];

#else

#define hard_smp_processor_id()		0

#endif

#endif
