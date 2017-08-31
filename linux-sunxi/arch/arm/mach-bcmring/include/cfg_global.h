/*
 * arch/arm/mach-bcmring/include/cfg_global.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _CFG_GLOBAL_H_
#define _CFG_GLOBAL_H_

#include <cfg_global_defines.h>

#define CFG_GLOBAL_CHIP                         BCM11107
#define CFG_GLOBAL_CHIP_FAMILY                  CFG_GLOBAL_CHIP_FAMILY_BCMRING
#define CFG_GLOBAL_CHIP_REV                     0xB0
#define CFG_GLOBAL_RAM_SIZE                     0x10000000
#define CFG_GLOBAL_RAM_BASE                     0x00000000
#define CFG_GLOBAL_RAM_RESERVED_SIZE            0x000000

#endif /* _CFG_GLOBAL_H_ */
