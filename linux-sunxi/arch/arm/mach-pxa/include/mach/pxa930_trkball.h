/*
 * arch/arm/mach-pxa/include/mach/pxa930_trkball.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_ARCH_PXA930_TRKBALL_H
#define __ASM_ARCH_PXA930_TRKBALL_H

struct pxa930_trkball_platform_data {
	int x_filter;
	int y_filter;
};

#endif /* __ASM_ARCH_PXA930_TRKBALL_H */

