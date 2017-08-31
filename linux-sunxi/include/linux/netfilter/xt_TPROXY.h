/*
 * include/linux/netfilter/xt_TPROXY.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_TPROXY_H
#define _XT_TPROXY_H

#include <linux/types.h>

/* TPROXY target is capable of marking the packet to perform
 * redirection. We can get rid of that whenever we get support for
 * mutliple targets in the same rule. */
struct xt_tproxy_target_info {
	__u32 mark_mask;
	__u32 mark_value;
	__be32 laddr;
	__be16 lport;
};

struct xt_tproxy_target_info_v1 {
	__u32 mark_mask;
	__u32 mark_value;
	union nf_inet_addr laddr;
	__be16 lport;
};

#endif /* _XT_TPROXY_H */
