/*
 * include/linux/crc7.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_CRC7_H
#define _LINUX_CRC7_H
#include <linux/types.h>

extern const u8 crc7_syndrome_table[256];

static inline u8 crc7_byte(u8 crc, u8 data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}

extern u8 crc7(u8 crc, const u8 *buffer, size_t len);

#endif
