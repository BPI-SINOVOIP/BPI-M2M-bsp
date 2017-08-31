/*
 * arch/mips/include/asm/lasat/lasatint.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_LASAT_LASATINT_H
#define __ASM_LASAT_LASATINT_H

/* lasat 100 */
#define LASAT_INT_STATUS_REG_100	(KSEG1ADDR(0x1c880000))
#define LASAT_INT_MASK_REG_100		(KSEG1ADDR(0x1c890000))
#define LASATINT_MASK_SHIFT_100		0

/* lasat 200 */
#define LASAT_INT_STATUS_REG_200	(KSEG1ADDR(0x1104003c))
#define LASAT_INT_MASK_REG_200		(KSEG1ADDR(0x1104003c))
#define LASATINT_MASK_SHIFT_200		16

#endif /* __ASM_LASAT_LASATINT_H */
