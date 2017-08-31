/*
 * include/linux/netfilter_ipv4/ipt_CLUSTERIP.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _IPT_CLUSTERIP_H_target
#define _IPT_CLUSTERIP_H_target

#include <linux/types.h>

enum clusterip_hashmode {
    CLUSTERIP_HASHMODE_SIP = 0,
    CLUSTERIP_HASHMODE_SIP_SPT,
    CLUSTERIP_HASHMODE_SIP_SPT_DPT,
};

#define CLUSTERIP_HASHMODE_MAX CLUSTERIP_HASHMODE_SIP_SPT_DPT

#define CLUSTERIP_MAX_NODES 16

#define CLUSTERIP_FLAG_NEW 0x00000001

struct clusterip_config;

struct ipt_clusterip_tgt_info {

	__u32 flags;

	/* only relevant for new ones */
	__u8 clustermac[6];
	__u16 num_total_nodes;
	__u16 num_local_nodes;
	__u16 local_nodes[CLUSTERIP_MAX_NODES];
	__u32 hash_mode;
	__u32 hash_initval;

	/* Used internally by the kernel */
	struct clusterip_config *config;
};

#endif /*_IPT_CLUSTERIP_H_target*/
