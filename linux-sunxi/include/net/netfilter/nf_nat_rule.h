/*
 * include/net/netfilter/nf_nat_rule.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_NAT_RULE_H
#define _NF_NAT_RULE_H
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_nat.h>
#include <linux/netfilter_ipv4/ip_tables.h>

extern int nf_nat_rule_init(void) __init;
extern void nf_nat_rule_cleanup(void);
extern int nf_nat_rule_find(struct sk_buff *skb,
			    unsigned int hooknum,
			    const struct net_device *in,
			    const struct net_device *out,
			    struct nf_conn *ct);

#endif /* _NF_NAT_RULE_H */
