/*
 * lib/bcd.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/bcd.h>
#include <linux/export.h>

unsigned bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}
EXPORT_SYMBOL(bcd2bin);

unsigned char bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}
EXPORT_SYMBOL(bin2bcd);
