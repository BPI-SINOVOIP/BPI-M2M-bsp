/*
 * include/linux/bcd.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _BCD_H
#define _BCD_H

#include <linux/compiler.h>

unsigned bcd2bin(unsigned char val) __attribute_const__;
unsigned char bin2bcd(unsigned val) __attribute_const__;

#endif /* _BCD_H */
