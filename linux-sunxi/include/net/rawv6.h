/*
 * include/net/rawv6.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NET_RAWV6_H
#define _NET_RAWV6_H

#include <net/protocol.h>

void raw6_icmp_error(struct sk_buff *, int nexthdr,
		u8 type, u8 code, int inner_offset, __be32);
int raw6_local_deliver(struct sk_buff *, int);

extern int			rawv6_rcv(struct sock *sk,
					  struct sk_buff *skb);

#if defined(CONFIG_IPV6_MIP6) || defined(CONFIG_IPV6_MIP6_MODULE)
int rawv6_mh_filter_register(int (*filter)(struct sock *sock,
					   struct sk_buff *skb));
int rawv6_mh_filter_unregister(int (*filter)(struct sock *sock,
					     struct sk_buff *skb));
#endif

#endif
