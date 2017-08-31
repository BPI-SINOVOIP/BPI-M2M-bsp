/*
 * net/core/kmap_skb.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/highmem.h>

static inline void *kmap_skb_frag(const skb_frag_t *frag)
{
#ifdef CONFIG_HIGHMEM
	BUG_ON(in_irq());

	local_bh_disable();
#endif
	return kmap_atomic(skb_frag_page(frag));
}

static inline void kunmap_skb_frag(void *vaddr)
{
	kunmap_atomic(vaddr);
#ifdef CONFIG_HIGHMEM
	local_bh_enable();
#endif
}
