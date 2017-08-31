/*
 * include/linux/netfilter/xt_layer7.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_LAYER7_H
#define _XT_LAYER7_H

#define MAX_PATTERN_LEN 8192
#define MAX_PROTOCOL_LEN 256

struct xt_layer7_info {
    char protocol[MAX_PROTOCOL_LEN];
    char pattern[MAX_PATTERN_LEN];
    u_int8_t invert;
    u_int8_t pkt;
};

#endif /* _XT_LAYER7_H */
