/*
 * include/linux/netfilter/xt_CT.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_CT_H
#define _XT_CT_H

#include <linux/types.h>

#define XT_CT_NOTRACK	0x1

struct xt_ct_target_info {
	__u16 flags;
	__u16 zone;
	__u32 ct_events;
	__u32 exp_events;
	char helper[16];

	/* Used internally by the kernel */
	struct nf_conn	*ct __attribute__((aligned(8)));
};

struct xt_ct_target_info_v1 {
	__u16 flags;
	__u16 zone;
	__u32 ct_events;
	__u32 exp_events;
	char helper[16];
	char timeout[32];

	/* Used internally by the kernel */
	struct nf_conn	*ct __attribute__((aligned(8)));
};

#endif /* _XT_CT_H */
