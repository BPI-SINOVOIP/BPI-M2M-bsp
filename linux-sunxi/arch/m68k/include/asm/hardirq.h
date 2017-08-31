/*
 * arch/m68k/include/asm/hardirq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __M68K_HARDIRQ_H
#define __M68K_HARDIRQ_H

#include <linux/threads.h>
#include <linux/cache.h>
#include <asm/irq.h>

#define HARDIRQ_BITS	8

/*
 * The hardirq mask has to be large enough to have
 * space for potentially all IRQ sources in the system
 * nesting on a single CPU:
 */
#if (1 << HARDIRQ_BITS) < NR_IRQS
# error HARDIRQ_BITS is too low!
#endif

#ifdef CONFIG_MMU

static inline void ack_bad_irq(unsigned int irq)
{
	pr_crit("unexpected IRQ trap at vector %02x\n", irq);
}

/* entry.S is sensitive to the offsets of these fields */
typedef struct {
	unsigned int __softirq_pending;
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */

#else

#include <asm-generic/hardirq.h>

#endif /* !CONFIG_MMU */

#endif
