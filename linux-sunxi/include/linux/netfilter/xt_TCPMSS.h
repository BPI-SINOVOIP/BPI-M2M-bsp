/*
 * include/linux/netfilter/xt_TCPMSS.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_TCPMSS_H
#define _XT_TCPMSS_H

#include <linux/types.h>

struct xt_tcpmss_info {
	__u16 mss;
};

#define XT_TCPMSS_CLAMP_PMTU 0xffff

#endif /* _XT_TCPMSS_H */
