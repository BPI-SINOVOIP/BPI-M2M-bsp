/*
 * include/linux/netfilter/xt_string.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_STRING_H
#define _XT_STRING_H

#include <linux/types.h>

#define XT_STRING_MAX_PATTERN_SIZE 128
#define XT_STRING_MAX_ALGO_NAME_SIZE 16

enum {
	XT_STRING_FLAG_INVERT		= 0x01,
	XT_STRING_FLAG_IGNORECASE	= 0x02
};

struct xt_string_info {
	__u16 from_offset;
	__u16 to_offset;
	char	  algo[XT_STRING_MAX_ALGO_NAME_SIZE];
	char 	  pattern[XT_STRING_MAX_PATTERN_SIZE];
	__u8  patlen;
	union {
		struct {
			__u8  invert;
		} v0;

		struct {
			__u8  flags;
		} v1;
	} u;

	/* Used internally by the kernel */
	struct ts_config __attribute__((aligned(8))) *config;
};

#endif /*_XT_STRING_H*/
