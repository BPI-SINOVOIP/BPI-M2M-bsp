/*
 * arch/unicore32/include/mach/regs-intc.h
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
 * PKUNITY Interrupt Controller (INTC) Registers
 */
/*
 * INTC Level Reg INTC_ICLR.
 */
#define INTC_ICLR	(PKUNITY_INTC_BASE + 0x0000)
/*
 * INTC Mask Reg INTC_ICMR.
 */
#define INTC_ICMR	(PKUNITY_INTC_BASE + 0x0004)
/*
 * INTC Pending Reg INTC_ICPR.
 */
#define INTC_ICPR	(PKUNITY_INTC_BASE + 0x0008)
/*
 * INTC IRQ Pending Reg INTC_ICIP.
 */
#define INTC_ICIP	(PKUNITY_INTC_BASE + 0x000C)
/*
 * INTC REAL Pending Reg INTC_ICFP.
 */
#define INTC_ICFP	(PKUNITY_INTC_BASE + 0x0010)
/*
 * INTC Control Reg INTC_ICCR.
 */
#define INTC_ICCR	(PKUNITY_INTC_BASE + 0x0014)

