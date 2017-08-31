/*
 * net/compat-wireless/include/linux/compat-2.6.21.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_26_21_COMPAT_H
#define LINUX_26_21_COMPAT_H

#include <linux/version.h>

/* Compat work for 2.6.21 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21))

#include <linux/sysctl.h>

#define register_sysctl_table(table)				\
	({							\
		register_sysctl_table((table), 0);		\
	})

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)) */

#endif /* LINUX_26_21_COMPAT_H */
