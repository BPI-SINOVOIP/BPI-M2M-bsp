/*
 * arch/microblaze/include/asm/byteorder.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_MICROBLAZE_BYTEORDER_H
#define _ASM_MICROBLAZE_BYTEORDER_H

#ifdef __MICROBLAZEEL__
#include <linux/byteorder/little_endian.h>
#else
#include <linux/byteorder/big_endian.h>
#endif

#endif /* _ASM_MICROBLAZE_BYTEORDER_H */
