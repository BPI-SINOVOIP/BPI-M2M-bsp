/*
 * net/compat-wireless/include/linux/compat-2.6.19.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_26_19_COMPAT_H
#define LINUX_26_19_COMPAT_H

#include <linux/version.h>

/* Compat work for 2.6.19 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19))

#include <linux/slab.h>

static inline int
compat_kmem_cache_destroy(struct kmem_cache *cachep)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19))
	return kmem_cache_destroy(cachep);
#else
	kmem_cache_destroy(cachep);
	return 0;
#endif
}

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)) */

#endif /* LINUX_26_19_COMPAT_H */
