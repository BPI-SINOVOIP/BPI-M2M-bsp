/*
 * include/linux/netfilter/xt_u32.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_U32_H
#define _XT_U32_H 1

#include <linux/types.h>

enum xt_u32_ops {
	XT_U32_AND,
	XT_U32_LEFTSH,
	XT_U32_RIGHTSH,
	XT_U32_AT,
};

struct xt_u32_location_element {
	__u32 number;
	__u8 nextop;
};

struct xt_u32_value_element {
	__u32 min;
	__u32 max;
};

/*
 * Any way to allow for an arbitrary number of elements?
 * For now, I settle with a limit of 10 each.
 */
#define XT_U32_MAXSIZE 10

struct xt_u32_test {
	struct xt_u32_location_element location[XT_U32_MAXSIZE+1];
	struct xt_u32_value_element value[XT_U32_MAXSIZE+1];
	__u8 nnums;
	__u8 nvalues;
};

struct xt_u32 {
	struct xt_u32_test tests[XT_U32_MAXSIZE+1];
	__u8 ntests;
	__u8 invert;
};

#endif /* _XT_U32_H */
