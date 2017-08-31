/*
 * net/compat-wireless/include/net/net_namespace.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _COMPAT_NET_NET_NAMESPACE_H
#define _COMPAT_NET_NET_NAMESPACE_H 1

#include <linux/version.h>

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23))
#include_next <net/net_namespace.h>
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)) */

#endif	/* _COMPAT_NET_NET_NAMESPACE_H */
