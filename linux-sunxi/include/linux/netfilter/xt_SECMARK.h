/*
 * include/linux/netfilter/xt_SECMARK.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _XT_SECMARK_H_target
#define _XT_SECMARK_H_target

#include <linux/types.h>

/*
 * This is intended for use by various security subsystems (but not
 * at the same time).
 *
 * 'mode' refers to the specific security subsystem which the
 * packets are being marked for.
 */
#define SECMARK_MODE_SEL	0x01		/* SELinux */
#define SECMARK_SECCTX_MAX	256

struct xt_secmark_target_info {
	__u8 mode;
	__u32 secid;
	char secctx[SECMARK_SECCTX_MAX];
};

#endif /*_XT_SECMARK_H_target */
