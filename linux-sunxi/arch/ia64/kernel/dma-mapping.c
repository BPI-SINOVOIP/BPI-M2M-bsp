/*
 * arch/ia64/kernel/dma-mapping.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/dma-mapping.h>
#include <linux/export.h>

/* Set this to 1 if there is a HW IOMMU in the system */
int iommu_detected __read_mostly;

struct dma_map_ops *dma_ops;
EXPORT_SYMBOL(dma_ops);

#define PREALLOC_DMA_DEBUG_ENTRIES (1 << 16)

static int __init dma_init(void)
{
	dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);

	return 0;
}
fs_initcall(dma_init);

struct dma_map_ops *dma_get_ops(struct device *dev)
{
	return dma_ops;
}
EXPORT_SYMBOL(dma_get_ops);
