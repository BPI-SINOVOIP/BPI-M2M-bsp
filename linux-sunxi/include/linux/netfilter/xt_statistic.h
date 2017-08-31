/*
 * include/linux/netfilter/xt_statistic.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_STATISTIC_H
#define _XT_STATISTIC_H

#include <linux/types.h>

enum xt_statistic_mode {
	XT_STATISTIC_MODE_RANDOM,
	XT_STATISTIC_MODE_NTH,
	__XT_STATISTIC_MODE_MAX
};
#define XT_STATISTIC_MODE_MAX (__XT_STATISTIC_MODE_MAX - 1)

enum xt_statistic_flags {
	XT_STATISTIC_INVERT		= 0x1,
};
#define XT_STATISTIC_MASK		0x1

struct xt_statistic_priv;

struct xt_statistic_info {
	__u16			mode;
	__u16			flags;
	union {
		struct {
			__u32	probability;
		} random;
		struct {
			__u32	every;
			__u32	packet;
			__u32	count; /* unused */
		} nth;
	} u;
	struct xt_statistic_priv *master __attribute__((aligned(8)));
};

#endif /* _XT_STATISTIC_H */
