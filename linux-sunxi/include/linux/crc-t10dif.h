/*
 * include/linux/crc-t10dif.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_CRC_T10DIF_H
#define _LINUX_CRC_T10DIF_H

#include <linux/types.h>

__u16 crc_t10dif(unsigned char const *, size_t);

#endif
