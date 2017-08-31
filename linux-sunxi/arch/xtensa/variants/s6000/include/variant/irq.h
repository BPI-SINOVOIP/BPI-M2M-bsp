/*
 * arch/xtensa/variants/s6000/include/variant/irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XTENSA_S6000_IRQ_H
#define _XTENSA_S6000_IRQ_H

#define NO_IRQ		(-1)
#define VARIANT_NR_IRQS 8 /* GPIO interrupts */

extern void variant_irq_enable(unsigned int irq);

#endif /* __XTENSA_S6000_IRQ_H */
