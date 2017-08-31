/*
 * include/linux/tc_ematch/tc_em_text.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_TC_EM_TEXT_H
#define __LINUX_TC_EM_TEXT_H

#include <linux/types.h>
#include <linux/pkt_cls.h>

#define TC_EM_TEXT_ALGOSIZ	16

struct tcf_em_text {
	char		algo[TC_EM_TEXT_ALGOSIZ];
	__u16		from_offset;
	__u16		to_offset;
	__u16		pattern_len;
	__u8		from_layer:4;
	__u8		to_layer:4;
	__u8		pad;
};

#endif
