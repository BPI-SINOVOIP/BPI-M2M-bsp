/*
 * arch/arm/include/asm/device.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
 */
#ifndef ASMARM_DEVICE_H
#define ASMARM_DEVICE_H

struct dev_archdata {
	struct dma_map_ops	*dma_ops;
#ifdef CONFIG_DMABOUNCE
	struct dmabounce_device_info *dmabounce;
#endif
#ifdef CONFIG_IOMMU_API
	void *iommu; /* private IOMMU data */
#endif
#ifdef CONFIG_ARM_DMA_USE_IOMMU
	struct dma_iommu_mapping	*mapping;
#endif
};

struct omap_device;

struct pdev_archdata {
#ifdef CONFIG_ARCH_OMAP
	struct omap_device *od;
#endif
};

#endif
