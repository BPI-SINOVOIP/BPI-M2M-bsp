/*
 * arch/arm/mach-pxa/include/mach/littleton.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARCH_LITTLETON_H
#define __ASM_ARCH_LITTLETON_H

#define LITTLETON_ETH_PHYS	0x30000000

#define LITTLETON_GPIO_LCD_CS	(17)

#define EXT0_GPIO_BASE	(PXA_NR_BUILTIN_GPIO)
#define EXT0_GPIO(x)	(EXT0_GPIO_BASE + (x))

#define LITTLETON_NR_IRQS	(IRQ_BOARD_START + 8)

#endif /* __ASM_ARCH_LITTLETON_H */
