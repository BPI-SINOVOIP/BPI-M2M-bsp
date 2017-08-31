/*
 * arch/xtensa/include/asm/byteorder.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XTENSA_BYTEORDER_H
#define _XTENSA_BYTEORDER_H

#ifdef __XTENSA_EL__
#include <linux/byteorder/little_endian.h>
#elif defined(__XTENSA_EB__)
#include <linux/byteorder/big_endian.h>
#else
# error processor byte order undefined!
#endif

#endif /* _XTENSA_BYTEORDER_H */
