/*
 * include/linux/netfilter/xt_rpfilter.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_RPATH_H
#define _XT_RPATH_H

#include <linux/types.h>

enum {
	XT_RPFILTER_LOOSE = 1 << 0,
	XT_RPFILTER_VALID_MARK = 1 << 1,
	XT_RPFILTER_ACCEPT_LOCAL = 1 << 2,
	XT_RPFILTER_INVERT = 1 << 3,
#ifdef __KERNEL__
	XT_RPFILTER_OPTION_MASK = XT_RPFILTER_LOOSE |
				  XT_RPFILTER_VALID_MARK |
				  XT_RPFILTER_ACCEPT_LOCAL |
				  XT_RPFILTER_INVERT,
#endif
};

struct xt_rpfilter_info {
	__u8 flags;
};

#endif
