/*
 * include/linux/netfilter/xt_cluster.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_CLUSTER_MATCH_H
#define _XT_CLUSTER_MATCH_H

#include <linux/types.h>

enum xt_cluster_flags {
	XT_CLUSTER_F_INV	= (1 << 0)
};

struct xt_cluster_match_info {
	__u32 total_nodes;
	__u32 node_mask;
	__u32 hash_seed;
	__u32 flags;
};

#define XT_CLUSTER_NODES_MAX	32

#endif /* _XT_CLUSTER_MATCH_H */
