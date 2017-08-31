/*
 * include/linux/netfilter/ipset/ip_set_bitmap.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __IP_SET_BITMAP_H
#define __IP_SET_BITMAP_H

/* Bitmap type specific error codes */
enum {
	/* The element is out of the range of the set */
	IPSET_ERR_BITMAP_RANGE = IPSET_ERR_TYPE_SPECIFIC,
	/* The range exceeds the size limit of the set type */
	IPSET_ERR_BITMAP_RANGE_SIZE,
};

#ifdef __KERNEL__
#define IPSET_BITMAP_MAX_RANGE	0x0000FFFF

/* Common functions */

static inline u32
range_to_mask(u32 from, u32 to, u8 *bits)
{
	u32 mask = 0xFFFFFFFE;

	*bits = 32;
	while (--(*bits) > 0 && mask && (to & mask) != from)
		mask <<= 1;

	return mask;
}

#endif /* __KERNEL__ */

#endif /* __IP_SET_BITMAP_H */
