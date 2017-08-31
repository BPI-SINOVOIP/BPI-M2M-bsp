/*
 * arch/mips/include/asm/lasat/head.h
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
 * Image header stuff
 */
#ifndef _HEAD_H
#define _HEAD_H

#define LASAT_K_MAGIC0_VAL	0xfedeabba
#define LASAT_K_MAGIC1_VAL	0x00bedead

#ifndef _LANGUAGE_ASSEMBLY
#include <linux/types.h>
struct bootloader_header {
	u32 magic[2];
	u32 version;
	u32 image_start;
	u32 image_size;
	u32 kernel_start;
	u32 kernel_entry;
};
#endif

#endif /* _HEAD_H */
