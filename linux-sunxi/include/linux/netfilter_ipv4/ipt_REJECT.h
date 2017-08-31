/*
 * include/linux/netfilter_ipv4/ipt_REJECT.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _IPT_REJECT_H
#define _IPT_REJECT_H

enum ipt_reject_with {
	IPT_ICMP_NET_UNREACHABLE,
	IPT_ICMP_HOST_UNREACHABLE,
	IPT_ICMP_PROT_UNREACHABLE,
	IPT_ICMP_PORT_UNREACHABLE,
	IPT_ICMP_ECHOREPLY,
	IPT_ICMP_NET_PROHIBITED,
	IPT_ICMP_HOST_PROHIBITED,
	IPT_TCP_RESET,
	IPT_ICMP_ADMIN_PROHIBITED
};

struct ipt_reject_info {
	enum ipt_reject_with with;      /* reject type */
};

#endif /*_IPT_REJECT_H*/
