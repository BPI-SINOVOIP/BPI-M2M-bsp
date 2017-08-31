/*
 * include/asm-generic/dma-mapping-broken.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_GENERIC_DMA_MAPPING_H
#define _ASM_GENERIC_DMA_MAPPING_H

/* define the dma api to allow compilation but not linking of
 * dma dependent code.  Code that depends on the dma-mapping
 * API needs to set 'depends on HAS_DMA' in its Kconfig
 */

struct scatterlist;

extern void *
dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle,
		   gfp_t flag);

extern void
dma_free_coherent(struct device *dev, size_t size, void *cpu_addr,
		    dma_addr_t dma_handle);

#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

extern dma_addr_t
dma_map_single(struct device *dev, void *ptr, size_t size,
	       enum dma_data_direction direction);

extern void
dma_unmap_single(struct device *dev, dma_addr_t dma_addr, size_t size,
		 enum dma_data_direction direction);

extern int
dma_map_sg(struct device *dev, struct scatterlist *sg, int nents,
	   enum dma_data_direction direction);

extern void
dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nhwentries,
	     enum dma_data_direction direction);

extern dma_addr_t
dma_map_page(struct device *dev, struct page *page, unsigned long offset,
	     size_t size, enum dma_data_direction direction);

extern void
dma_unmap_page(struct device *dev, dma_addr_t dma_address, size_t size,
	       enum dma_data_direction direction);

extern void
dma_sync_single_for_cpu(struct device *dev, dma_addr_t dma_handle, size_t size,
			enum dma_data_direction direction);

extern void
dma_sync_single_range_for_cpu(struct device *dev, dma_addr_t dma_handle,
			      unsigned long offset, size_t size,
			      enum dma_data_direction direction);

extern void
dma_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg, int nelems,
		    enum dma_data_direction direction);

#define dma_sync_single_for_device dma_sync_single_for_cpu
#define dma_sync_single_range_for_device dma_sync_single_range_for_cpu
#define dma_sync_sg_for_device dma_sync_sg_for_cpu

extern int
dma_mapping_error(struct device *dev, dma_addr_t dma_addr);

extern int
dma_supported(struct device *dev, u64 mask);

extern int
dma_set_mask(struct device *dev, u64 mask);

extern int
dma_get_cache_alignment(void);

extern void
dma_cache_sync(struct device *dev, void *vaddr, size_t size,
	       enum dma_data_direction direction);

#endif /* _ASM_GENERIC_DMA_MAPPING_H */
