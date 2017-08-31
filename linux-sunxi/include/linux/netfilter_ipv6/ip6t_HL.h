/*
 * include/linux/netfilter_ipv6/ip6t_HL.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Hop Limit modification module for ip6tables
 * Maciej Soltysiak <solt@dns.toxicfilms.tv>
 * Based on HW's TTL module */

#ifndef _IP6T_HL_H
#define _IP6T_HL_H

#include <linux/types.h>

enum {
	IP6T_HL_SET = 0,
	IP6T_HL_INC,
	IP6T_HL_DEC
};

#define IP6T_HL_MAXMODE	IP6T_HL_DEC

struct ip6t_HL_info {
	__u8	mode;
	__u8	hop_limit;
};


#endif
