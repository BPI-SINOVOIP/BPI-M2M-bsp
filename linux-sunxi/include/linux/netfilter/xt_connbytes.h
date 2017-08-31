/*
 * include/linux/netfilter/xt_connbytes.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_CONNBYTES_H
#define _XT_CONNBYTES_H

#include <linux/types.h>

enum xt_connbytes_what {
	XT_CONNBYTES_PKTS,
	XT_CONNBYTES_BYTES,
	XT_CONNBYTES_AVGPKT,
};

enum xt_connbytes_direction {
	XT_CONNBYTES_DIR_ORIGINAL,
	XT_CONNBYTES_DIR_REPLY,
	XT_CONNBYTES_DIR_BOTH,
};

struct xt_connbytes_info {
	struct {
		__aligned_u64 from;	/* count to be matched */
		__aligned_u64 to;	/* count to be matched */
	} count;
	__u8 what;		/* ipt_connbytes_what */
	__u8 direction;	/* ipt_connbytes_direction */
};
#endif
