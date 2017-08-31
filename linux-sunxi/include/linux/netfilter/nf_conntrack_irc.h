/*
 * include/linux/netfilter/nf_conntrack_irc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_CONNTRACK_IRC_H
#define _NF_CONNTRACK_IRC_H

#ifdef __KERNEL__

#define IRC_PORT	6667

extern unsigned int (*nf_nat_irc_hook)(struct sk_buff *skb,
				       enum ip_conntrack_info ctinfo,
				       unsigned int matchoff,
				       unsigned int matchlen,
				       struct nf_conntrack_expect *exp);

#endif /* __KERNEL__ */
#endif /* _NF_CONNTRACK_IRC_H */
