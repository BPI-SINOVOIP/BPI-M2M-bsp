/*
 * include/linux/crc-ccitt.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_CRC_CCITT_H
#define _LINUX_CRC_CCITT_H

#include <linux/types.h>

extern u16 const crc_ccitt_table[256];

extern u16 crc_ccitt(u16 crc, const u8 *buffer, size_t len);

static inline u16 crc_ccitt_byte(u16 crc, const u8 c)
{
	return (crc >> 8) ^ crc_ccitt_table[(crc ^ c) & 0xff];
}

#endif /* _LINUX_CRC_CCITT_H */
