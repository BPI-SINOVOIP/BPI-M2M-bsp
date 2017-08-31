/*
 * arch/arm/plat-orion/include/plat/mv_xor.h
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
 * arch/arm/plat-orion/include/plat/mv_xor.h
 *
 * Marvell XOR platform device data definition file.
 */

#ifndef __PLAT_MV_XOR_H
#define __PLAT_MV_XOR_H

#include <linux/dmaengine.h>
#include <linux/mbus.h>

#define MV_XOR_SHARED_NAME	"mv_xor_shared"
#define MV_XOR_NAME		"mv_xor"

struct mv_xor_platform_data {
	struct platform_device		*shared;
	int				hw_id;
	dma_cap_mask_t			cap_mask;
	size_t				pool_size;
};


#endif
