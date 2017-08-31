/*
 * arch/ia64/kernel/nr-irqs.c
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
 * calculate
 * NR_IRQS = max(IA64_NATIVE_NR_IRQS, XEN_NR_IRQS, FOO_NR_IRQS...)
 * depending on config.
 * This must be calculated before processing asm-offset.c.
 */

#define ASM_OFFSETS_C 1

#include <linux/kbuild.h>
#include <linux/threads.h>
#include <asm/native/irq.h>
#include <asm/xen/irq.h>

void foo(void)
{
	union paravirt_nr_irqs_max {
		char ia64_native_nr_irqs[IA64_NATIVE_NR_IRQS];
#ifdef CONFIG_XEN
		char xen_nr_irqs[XEN_NR_IRQS];
#endif
	};

	DEFINE(NR_IRQS, sizeof (union paravirt_nr_irqs_max));
}
