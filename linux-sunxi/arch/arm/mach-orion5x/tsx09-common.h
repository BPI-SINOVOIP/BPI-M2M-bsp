/*
 * arch/arm/mach-orion5x/tsx09-common.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ARCH_ORION5X_TSX09_COMMON_H
#define __ARCH_ORION5X_TSX09_COMMON_H

/*
 * QNAP TS-x09 Boards power-off function
 */
extern void qnap_tsx09_power_off(void);

/*
 * QNAP TS-x09 Boards function to find Ethernet MAC address in flash memory
 */
extern void __init qnap_tsx09_find_mac_addr(u32 mem_base, u32 size);

/*
 * QNAP TS-x09 Boards ethernet declaration
 */
extern struct mv643xx_eth_platform_data qnap_tsx09_eth_data;


#endif
