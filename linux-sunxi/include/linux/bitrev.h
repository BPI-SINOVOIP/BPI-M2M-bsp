/*
 * include/linux/bitrev.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_BITREV_H
#define _LINUX_BITREV_H

#include <linux/types.h>

extern u8 const byte_rev_table[256];

static inline u8 bitrev8(u8 byte)
{
	return byte_rev_table[byte];
}

extern u16 bitrev16(u16 in);
extern u32 bitrev32(u32 in);

#endif /* _LINUX_BITREV_H */
