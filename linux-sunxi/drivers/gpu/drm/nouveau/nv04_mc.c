/*
 * drivers/gpu/drm/nouveau/nv04_mc.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "drmP.h"
#include "drm.h"
#include "nouveau_drv.h"
#include "nouveau_drm.h"

int
nv04_mc_init(struct drm_device *dev)
{
	/* Power up everything, resetting each individual unit will
	 * be done later if needed.
	 */

	nv_wr32(dev, NV03_PMC_ENABLE, 0xFFFFFFFF);

	/* Disable PROM access. */
	nv_wr32(dev, NV_PBUS_PCI_NV_20, NV_PBUS_PCI_NV_20_ROM_SHADOW_ENABLED);

	return 0;
}

void
nv04_mc_takedown(struct drm_device *dev)
{
}
