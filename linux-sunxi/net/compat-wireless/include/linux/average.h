/*
 * net/compat-wireless/include/linux/average.h
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

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,37))
#include_next <linux/average.h>
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,37)) */
