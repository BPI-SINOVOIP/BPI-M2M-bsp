/*
 * arch/arm/mach-nomadik/include/mach/nand.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARCH_NAND_H
#define __ASM_ARCH_NAND_H

struct nomadik_nand_platform_data {
	struct mtd_partition *parts;
	int nparts;
	int options;
	int (*init) (void);
	int (*exit) (void);
};

#define NAND_IO_DATA	0x40000000
#define NAND_IO_CMD	0x40800000
#define NAND_IO_ADDR	0x41000000

#endif				/* __ASM_ARCH_NAND_H */
