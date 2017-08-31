/*
 * include/linux/netfilter_ipv4/ipt_addrtype.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _IPT_ADDRTYPE_H
#define _IPT_ADDRTYPE_H

#include <linux/types.h>

enum {
	IPT_ADDRTYPE_INVERT_SOURCE	= 0x0001,
	IPT_ADDRTYPE_INVERT_DEST	= 0x0002,
	IPT_ADDRTYPE_LIMIT_IFACE_IN	= 0x0004,
	IPT_ADDRTYPE_LIMIT_IFACE_OUT	= 0x0008,
};

struct ipt_addrtype_info_v1 {
	__u16	source;		/* source-type mask */
	__u16	dest;		/* dest-type mask */
	__u32	flags;
};

/* revision 0 */
struct ipt_addrtype_info {
	__u16	source;		/* source-type mask */
	__u16	dest;		/* dest-type mask */
	__u32	invert_source;
	__u32	invert_dest;
};

#endif
