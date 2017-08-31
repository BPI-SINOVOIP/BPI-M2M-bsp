/*
 * arch/powerpc/boot/cuboot.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PPC_BOOT_CUBOOT_H_
#define _PPC_BOOT_CUBOOT_H_

void cuboot_init(unsigned long r4, unsigned long r5,
		 unsigned long r6, unsigned long r7,
		 unsigned long end_of_ram);

#define CUBOOT_INIT() \
	do { \
		memcpy(&bd, (bd_t *)r3, sizeof(bd)); \
		cuboot_init(r4, r5, r6, r7, bd.bi_memstart + bd.bi_memsize); \
	} while (0)

#endif /* _PPC_BOOT_CUBOOT_H_ */
