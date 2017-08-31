/*
 * arch/mips/include/asm/mach-bcm63xx/bcm63xx_irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef BCM63XX_IRQ_H_
#define BCM63XX_IRQ_H_

#include <bcm63xx_cpu.h>

#define IRQ_INTERNAL_BASE		8
#define IRQ_EXTERNAL_BASE		100
#define IRQ_EXT_0			(IRQ_EXTERNAL_BASE + 0)
#define IRQ_EXT_1			(IRQ_EXTERNAL_BASE + 1)
#define IRQ_EXT_2			(IRQ_EXTERNAL_BASE + 2)
#define IRQ_EXT_3			(IRQ_EXTERNAL_BASE + 3)

#endif /* ! BCM63XX_IRQ_H_ */
