/*
 * include/linux/netfilter/xt_quota.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_QUOTA_H
#define _XT_QUOTA_H

#include <linux/types.h>

enum xt_quota_flags {
	XT_QUOTA_INVERT		= 0x1,
};
#define XT_QUOTA_MASK		0x1

struct xt_quota_priv;

struct xt_quota_info {
	__u32 flags;
	__u32 pad;
	__aligned_u64 quota;

	/* Used internally by the kernel */
	struct xt_quota_priv	*master;
};

#endif /* _XT_QUOTA_H */
