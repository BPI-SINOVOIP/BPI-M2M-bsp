/*
 * include/linux/tc_act/tc_nat.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_TC_NAT_H
#define __LINUX_TC_NAT_H

#include <linux/pkt_cls.h>
#include <linux/types.h>

#define TCA_ACT_NAT 9

enum {
	TCA_NAT_UNSPEC,
	TCA_NAT_PARMS,
	TCA_NAT_TM,
	__TCA_NAT_MAX
};
#define TCA_NAT_MAX (__TCA_NAT_MAX - 1)

#define TCA_NAT_FLAG_EGRESS 1

struct tc_nat {
	tc_gen;
	__be32 old_addr;
	__be32 new_addr;
	__be32 mask;
	__u32 flags;
};

#endif
