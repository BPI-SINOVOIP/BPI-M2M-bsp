/*
 * include/asm-generic/hardirq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_HARDIRQ_H
#define __ASM_GENERIC_HARDIRQ_H

#include <linux/cache.h>
#include <linux/threads.h>

typedef struct {
	unsigned int __softirq_pending;
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */
#include <linux/irq.h>

#ifndef ack_bad_irq
static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}
#endif

#endif /* __ASM_GENERIC_HARDIRQ_H */
