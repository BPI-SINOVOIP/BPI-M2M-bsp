/*
 * include/linux/netfilter/xt_length.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_LENGTH_H
#define _XT_LENGTH_H

#include <linux/types.h>

struct xt_length_info {
    __u16	min, max;
    __u8	invert;
};

#endif /*_XT_LENGTH_H*/
