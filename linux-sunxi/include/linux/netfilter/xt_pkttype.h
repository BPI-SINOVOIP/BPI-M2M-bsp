/*
 * include/linux/netfilter/xt_pkttype.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_PKTTYPE_H
#define _XT_PKTTYPE_H

struct xt_pkttype_info {
	int	pkttype;
	int	invert;
};
#endif /*_XT_PKTTYPE_H*/
