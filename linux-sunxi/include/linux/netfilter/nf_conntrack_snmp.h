/*
 * include/linux/netfilter/nf_conntrack_snmp.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NF_CONNTRACK_SNMP_H
#define _NF_CONNTRACK_SNMP_H

extern int (*nf_nat_snmp_hook)(struct sk_buff *skb,
				unsigned int protoff,
				struct nf_conn *ct,
				enum ip_conntrack_info ctinfo);

#endif /* _NF_CONNTRACK_SNMP_H */
