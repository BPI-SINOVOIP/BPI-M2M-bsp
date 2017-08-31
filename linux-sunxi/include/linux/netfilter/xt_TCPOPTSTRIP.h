/*
 * include/linux/netfilter/xt_TCPOPTSTRIP.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_TCPOPTSTRIP_H
#define _XT_TCPOPTSTRIP_H

#include <linux/types.h>

#define tcpoptstrip_set_bit(bmap, idx) \
	(bmap[(idx) >> 5] |= 1U << (idx & 31))
#define tcpoptstrip_test_bit(bmap, idx) \
	(((1U << (idx & 31)) & bmap[(idx) >> 5]) != 0)

struct xt_tcpoptstrip_target_info {
	__u32 strip_bmap[8];
};

#endif /* _XT_TCPOPTSTRIP_H */
