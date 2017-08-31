/*
 * include/linux/netfilter/xt_devgroup.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_DEVGROUP_H
#define _XT_DEVGROUP_H

#include <linux/types.h>

enum xt_devgroup_flags {
	XT_DEVGROUP_MATCH_SRC	= 0x1,
	XT_DEVGROUP_INVERT_SRC	= 0x2,
	XT_DEVGROUP_MATCH_DST	= 0x4,
	XT_DEVGROUP_INVERT_DST	= 0x8,
};

struct xt_devgroup_info {
	__u32	flags;
	__u32	src_group;
	__u32	src_mask;
	__u32	dst_group;
	__u32	dst_mask;
};

#endif /* _XT_DEVGROUP_H */
