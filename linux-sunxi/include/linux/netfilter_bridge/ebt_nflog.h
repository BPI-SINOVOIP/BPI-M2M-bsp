/*
 * include/linux/netfilter_bridge/ebt_nflog.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_BRIDGE_EBT_NFLOG_H
#define __LINUX_BRIDGE_EBT_NFLOG_H

#include <linux/types.h>

#define EBT_NFLOG_MASK 0x0

#define EBT_NFLOG_PREFIX_SIZE 64
#define EBT_NFLOG_WATCHER "nflog"

#define EBT_NFLOG_DEFAULT_GROUP		0x1
#define EBT_NFLOG_DEFAULT_THRESHOLD	1

struct ebt_nflog_info {
	__u32 len;
	__u16 group;
	__u16 threshold;
	__u16 flags;
	__u16 pad;
	char prefix[EBT_NFLOG_PREFIX_SIZE];
};

#endif				/* __LINUX_BRIDGE_EBT_NFLOG_H */
