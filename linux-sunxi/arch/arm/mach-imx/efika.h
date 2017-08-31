/*
 * arch/arm/mach-imx/efika.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _EFIKA_H
#define _EFIKA_H

#define EFIKA_WLAN_EN		IMX_GPIO_NR(2, 16)
#define EFIKA_WLAN_RESET	IMX_GPIO_NR(2, 10)
#define EFIKA_USB_PHY_RESET	IMX_GPIO_NR(2, 9)

void __init efika_board_common_init(void);

#endif
