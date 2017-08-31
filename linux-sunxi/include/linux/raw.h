/*
 * include/linux/raw.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_RAW_H
#define __LINUX_RAW_H

#include <linux/types.h>

#define RAW_SETBIND	_IO( 0xac, 0 )
#define RAW_GETBIND	_IO( 0xac, 1 )

struct raw_config_request 
{
	int	raw_minor;
	__u64	block_major;
	__u64	block_minor;
};

#define MAX_RAW_MINORS CONFIG_MAX_RAW_DEVS

#endif /* __LINUX_RAW_H */
