/*
 * include/linux/in_route.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_IN_ROUTE_H
#define _LINUX_IN_ROUTE_H

/* IPv4 routing cache flags */

#define RTCF_DEAD	RTNH_F_DEAD
#define RTCF_ONLINK	RTNH_F_ONLINK

/* Obsolete flag. About to be deleted */
#define RTCF_NOPMTUDISC RTM_F_NOPMTUDISC

#define RTCF_NOTIFY	0x00010000
#define RTCF_DIRECTDST	0x00020000 /* unused */
#define RTCF_REDIRECTED	0x00040000
#define RTCF_TPROXY	0x00080000 /* unused */

#define RTCF_FAST	0x00200000 /* unused */
#define RTCF_MASQ	0x00400000 /* unused */
#define RTCF_SNAT	0x00800000 /* unused */
#define RTCF_DOREDIRECT 0x01000000
#define RTCF_DIRECTSRC	0x04000000
#define RTCF_DNAT	0x08000000
#define RTCF_BROADCAST	0x10000000
#define RTCF_MULTICAST	0x20000000
#define RTCF_REJECT	0x40000000 /* unused */
#define RTCF_LOCAL	0x80000000

#define RTCF_NAT	(RTCF_DNAT|RTCF_SNAT)

#define RT_TOS(tos)	((tos)&IPTOS_TOS_MASK)

#endif /* _LINUX_IN_ROUTE_H */
