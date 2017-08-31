/*
 * arch/xtensa/platforms/s6105/include/platform/hardware.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __XTENSA_S6105_HARDWARE_H
#define __XTENSA_S6105_HARDWARE_H

#define PLATFORM_DEFAULT_MEM_START	0x40000000
#define PLATFORM_DEFAULT_MEM_SIZE	0x08000000

#define MAX_DMA_ADDRESS			0

#define KERNELOFFSET			(PLATFORM_DEFAULT_MEM_START + 0x1000)

#endif /* __XTENSA_S6105_HARDWARE_H */
