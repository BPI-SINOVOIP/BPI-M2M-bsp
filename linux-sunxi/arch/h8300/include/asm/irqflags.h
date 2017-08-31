/*
 * arch/h8300/include/asm/irqflags.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _H8300_IRQFLAGS_H
#define _H8300_IRQFLAGS_H

static inline unsigned long arch_local_save_flags(void)
{
	unsigned long flags;
	asm volatile ("stc ccr,%w0" : "=r" (flags));
	return flags;
}

static inline void arch_local_irq_disable(void)
{
	asm volatile ("orc  #0x80,ccr" : : : "memory");
}

static inline void arch_local_irq_enable(void)
{
	asm volatile ("andc #0x7f,ccr" : : : "memory");
}

static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	asm volatile ("ldc %w0,ccr" : : "r" (flags) : "memory");
}

static inline bool arch_irqs_disabled_flags(unsigned long flags)
{
	return (flags & 0x80) == 0x80;
}

static inline bool arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* _H8300_IRQFLAGS_H */
