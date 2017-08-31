/*
 * include/linux/netfilter_ipv4/ipt_ah.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _IPT_AH_H
#define _IPT_AH_H

#include <linux/types.h>

struct ipt_ah {
	__u32 spis[2];			/* Security Parameter Index */
	__u8  invflags;			/* Inverse flags */
};



/* Values for "invflags" field in struct ipt_ah. */
#define IPT_AH_INV_SPI		0x01	/* Invert the sense of spi. */
#define IPT_AH_INV_MASK	0x01	/* All possible flags. */

#endif /*_IPT_AH_H*/
