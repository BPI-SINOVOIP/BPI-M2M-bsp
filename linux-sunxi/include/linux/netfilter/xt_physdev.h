/*
 * include/linux/netfilter/xt_physdev.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_PHYSDEV_H
#define _XT_PHYSDEV_H

#include <linux/types.h>

#ifdef __KERNEL__
#include <linux/if.h>
#endif

#define XT_PHYSDEV_OP_IN		0x01
#define XT_PHYSDEV_OP_OUT		0x02
#define XT_PHYSDEV_OP_BRIDGED		0x04
#define XT_PHYSDEV_OP_ISIN		0x08
#define XT_PHYSDEV_OP_ISOUT		0x10
#define XT_PHYSDEV_OP_MASK		(0x20 - 1)

struct xt_physdev_info {
	char physindev[IFNAMSIZ];
	char in_mask[IFNAMSIZ];
	char physoutdev[IFNAMSIZ];
	char out_mask[IFNAMSIZ];
	__u8 invert;
	__u8 bitmask;
};

#endif /*_XT_PHYSDEV_H*/
