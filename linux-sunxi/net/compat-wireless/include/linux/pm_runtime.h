/*
 * net/compat-wireless/include/linux/pm_runtime.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/version.h>

#ifndef __COMPAT_LINUX_PM_RUNTIME_H
#define __COMPAT_LINUX_PM_RUNTIME_H

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#include_next <linux/pm_runtime.h>
#else

static inline void pm_runtime_enable(struct device *dev) {}

#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)) */

#endif
