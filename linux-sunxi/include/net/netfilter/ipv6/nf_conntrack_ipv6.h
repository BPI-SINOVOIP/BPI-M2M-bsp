/*
 * include/net/netfilter/ipv6/nf_conntrack_ipv6.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_CONNTRACK_IPV6_H
#define _NF_CONNTRACK_IPV6_H

extern struct nf_conntrack_l3proto nf_conntrack_l3proto_ipv6;

extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_icmpv6;

#include <linux/sysctl.h>
extern struct ctl_table nf_ct_ipv6_sysctl_table[];

#endif /* _NF_CONNTRACK_IPV6_H*/
