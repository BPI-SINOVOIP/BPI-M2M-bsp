/*
 * include/linux/netfilter/ipset/ip_set_getport.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _IP_SET_GETPORT_H
#define _IP_SET_GETPORT_H

extern bool ip_set_get_ip4_port(const struct sk_buff *skb, bool src,
				__be16 *port, u8 *proto);

#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
extern bool ip_set_get_ip6_port(const struct sk_buff *skb, bool src,
				__be16 *port, u8 *proto);
#else
static inline bool ip_set_get_ip6_port(const struct sk_buff *skb, bool src,
				       __be16 *port, u8 *proto)
{
	return false;
}
#endif

extern bool ip_set_get_ip_port(const struct sk_buff *skb, u8 pf, bool src,
				__be16 *port);

static inline bool ip_set_proto_with_ports(u8 proto)
{
	switch (proto) {
	case IPPROTO_TCP:
	case IPPROTO_SCTP:
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
		return true;
	}
	return false;
}

#endif /*_IP_SET_GETPORT_H*/
