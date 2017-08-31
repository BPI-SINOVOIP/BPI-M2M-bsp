/*
 * include/net/netfilter/ipv6/nf_defrag_ipv6.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

extern void nf_defrag_ipv6_enable(void);

extern int nf_ct_frag6_init(void);
extern void nf_ct_frag6_cleanup(void);
extern struct sk_buff *nf_ct_frag6_gather(struct sk_buff *skb, u32 user);
extern void nf_ct_frag6_output(unsigned int hooknum, struct sk_buff *skb,
			       struct net_device *in,
			       struct net_device *out,
			       int (*okfn)(struct sk_buff *));

struct inet_frags_ctl;

#endif /* _NF_DEFRAG_IPV6_H */
