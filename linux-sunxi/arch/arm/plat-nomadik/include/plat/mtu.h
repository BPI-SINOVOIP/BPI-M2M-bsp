/*
 * arch/arm/plat-nomadik/include/plat/mtu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __PLAT_MTU_H
#define __PLAT_MTU_H

void nmdk_timer_init(void __iomem *base);
void nmdk_clkevt_reset(void);
void nmdk_clksrc_reset(void);

#endif /* __PLAT_MTU_H */

