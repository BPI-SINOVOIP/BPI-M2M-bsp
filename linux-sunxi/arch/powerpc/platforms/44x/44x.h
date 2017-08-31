/*
 * arch/powerpc/platforms/44x/44x.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __POWERPC_PLATFORMS_44X_44X_H
#define __POWERPC_PLATFORMS_44X_44X_H

extern u8 as1_readb(volatile u8 __iomem  *addr);
extern void as1_writeb(u8 data, volatile u8 __iomem *addr);

#define GPIO0_OSRH	0xC
#define GPIO0_TSRH	0x14
#define GPIO0_ISR1H	0x34

#endif /* __POWERPC_PLATFORMS_44X_44X_H */
