/*
 * include/linux/netfilter_ipv6/ip6t_hl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* ip6tables module for matching the Hop Limit value
 * Maciej Soltysiak <solt@dns.toxicfilms.tv>
 * Based on HW's ttl module */

#ifndef _IP6T_HL_H
#define _IP6T_HL_H

#include <linux/types.h>

enum {
	IP6T_HL_EQ = 0,		/* equals */
	IP6T_HL_NE,		/* not equals */
	IP6T_HL_LT,		/* less than */
	IP6T_HL_GT,		/* greater than */
};


struct ip6t_hl_info {
	__u8	mode;
	__u8	hop_limit;
};


#endif
