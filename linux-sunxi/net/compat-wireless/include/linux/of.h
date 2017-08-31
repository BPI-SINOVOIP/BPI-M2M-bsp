/*
 * net/compat-wireless/include/linux/of.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _COMPAT_LINUX_OF_H
#define _COMPAT_LINUX_OF_H 1

#include <linux/version.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34))
#include_next <linux/of.h>
#else

#ifdef CONFIG_OF
#include_next <linux/of.h>
#endif /* CONFIG_OF */

#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34)) */

#endif	/* _COMPAT_LINUX_OF_H */
