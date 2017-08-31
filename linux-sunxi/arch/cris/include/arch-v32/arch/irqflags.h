/*
 * arch/cris/include/arch-v32/arch/irqflags.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_CRIS_ARCH_IRQFLAGS_H
#define __ASM_CRIS_ARCH_IRQFLAGS_H

#include <linux/types.h>
#include <arch/ptrace.h>

static inline unsigned long arch_local_save_flags(void)
{
	unsigned long flags;
	asm volatile("move $ccs,%0" : "=rm" (flags) : : "memory");
	return flags;
}

static inline void arch_local_irq_disable(void)
{
	asm volatile("di" : : : "memory");
}

static inline void arch_local_irq_enable(void)
{
	asm volatile("ei" : : : "memory");
}

static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	asm volatile("move %0,$ccs" : : "rm" (flags) : "memory");
}

static inline bool arch_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & (1 << I_CCS_BITNR));
}

static inline bool arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* __ASM_CRIS_ARCH_IRQFLAGS_H */
