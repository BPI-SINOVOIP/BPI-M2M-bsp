/*
 * arch/arm/mach-nomadik/include/mach/fsmc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

/* Definitions for the Nomadik FSMC "Flexible Static Memory controller" */

#ifndef __ASM_ARCH_FSMC_H
#define __ASM_ARCH_FSMC_H

#include <mach/hardware.h>
/*
 * Register list
 */

/* bus control reg. and bus timing reg. for CS0..CS3 */
#define FSMC_BCR(x)     (NOMADIK_FSMC_VA + (x << 3))
#define FSMC_BTR(x)     (NOMADIK_FSMC_VA + (x << 3) + 0x04)

/* PC-card and NAND:
 * PCR = control register
 * PMEM = memory timing
 * PATT = attribute timing
 * PIO = I/O timing
 * PECCR = ECC result
 */
#define FSMC_PCR(x)     (NOMADIK_FSMC_VA + ((2 + x) << 5) + 0x00)
#define FSMC_PMEM(x)    (NOMADIK_FSMC_VA + ((2 + x) << 5) + 0x08)
#define FSMC_PATT(x)    (NOMADIK_FSMC_VA + ((2 + x) << 5) + 0x0c)
#define FSMC_PIO(x)     (NOMADIK_FSMC_VA + ((2 + x) << 5) + 0x10)
#define FSMC_PECCR(x)   (NOMADIK_FSMC_VA + ((2 + x) << 5) + 0x14)

#endif /* __ASM_ARCH_FSMC_H */
