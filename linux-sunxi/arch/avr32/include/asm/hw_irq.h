/*
 * arch/avr32/include/asm/hw_irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_HW_IRQ_H
#define __ASM_AVR32_HW_IRQ_H

static inline void hw_resend_irq(struct irq_chip *h, unsigned int i)
{
	/* Nothing to do */
}

#endif /* __ASM_AVR32_HW_IRQ_H */
