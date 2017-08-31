/*
 * include/linux/netfilter/xt_ipvs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_IPVS_H
#define _XT_IPVS_H

#include <linux/types.h>

enum {
	XT_IPVS_IPVS_PROPERTY =	1 << 0, /* all other options imply this one */
	XT_IPVS_PROTO =		1 << 1,
	XT_IPVS_VADDR =		1 << 2,
	XT_IPVS_VPORT =		1 << 3,
	XT_IPVS_DIR =		1 << 4,
	XT_IPVS_METHOD =	1 << 5,
	XT_IPVS_VPORTCTL =	1 << 6,
	XT_IPVS_MASK =		(1 << 7) - 1,
	XT_IPVS_ONCE_MASK =	XT_IPVS_MASK & ~XT_IPVS_IPVS_PROPERTY
};

struct xt_ipvs_mtinfo {
	union nf_inet_addr	vaddr, vmask;
	__be16			vport;
	__u8			l4proto;
	__u8			fwd_method;
	__be16			vportctl;

	__u8			invert;
	__u8			bitmask;
};

#endif /* _XT_IPVS_H */
