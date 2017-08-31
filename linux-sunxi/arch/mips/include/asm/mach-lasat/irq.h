/*
 * arch/mips/include/asm/mach-lasat/irq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_MACH_LASAT_IRQ_H
#define _ASM_MACH_LASAT_IRQ_H

#define LASAT_CASCADE_IRQ	(MIPS_CPU_IRQ_BASE + 2)

#define LASAT_IRQ_BASE		8
#define LASAT_IRQ_END		23

#define NR_IRQS			24

#include_next <irq.h>

#endif /* _ASM_MACH_LASAT_IRQ_H */
