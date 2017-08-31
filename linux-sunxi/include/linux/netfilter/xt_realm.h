/*
 * include/linux/netfilter/xt_realm.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_REALM_H
#define _XT_REALM_H

#include <linux/types.h>

struct xt_realm_info {
	__u32 id;
	__u32 mask;
	__u8 invert;
};

#endif /* _XT_REALM_H */
