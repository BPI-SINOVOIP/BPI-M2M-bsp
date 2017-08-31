/*
 * fs/ceph/ceph_frag.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Ceph 'frag' type
 */
#include <linux/module.h>
#include <linux/ceph/types.h>

int ceph_frag_compare(__u32 a, __u32 b)
{
	unsigned va = ceph_frag_value(a);
	unsigned vb = ceph_frag_value(b);
	if (va < vb)
		return -1;
	if (va > vb)
		return 1;
	va = ceph_frag_bits(a);
	vb = ceph_frag_bits(b);
	if (va < vb)
		return -1;
	if (va > vb)
		return 1;
	return 0;
}
