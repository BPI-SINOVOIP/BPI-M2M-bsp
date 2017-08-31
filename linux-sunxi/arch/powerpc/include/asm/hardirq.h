/*
 * arch/powerpc/include/asm/hardirq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_POWERPC_HARDIRQ_H
#define _ASM_POWERPC_HARDIRQ_H

#include <linux/threads.h>
#include <linux/irq.h>

typedef struct {
	unsigned int __softirq_pending;
	unsigned int timer_irqs;
	unsigned int pmu_irqs;
	unsigned int mce_exceptions;
	unsigned int spurious_irqs;
} ____cacheline_aligned irq_cpustat_t;

DECLARE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);

#define __ARCH_IRQ_STAT

#define local_softirq_pending()	__get_cpu_var(irq_stat).__softirq_pending

static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}

extern u64 arch_irq_stat_cpu(unsigned int cpu);
#define arch_irq_stat_cpu	arch_irq_stat_cpu

#endif /* _ASM_POWERPC_HARDIRQ_H */
