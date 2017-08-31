/*
 * arch/arm/mach-omap2/include/mach/board-zoom.h
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
 * Defines for zoom boards
 */
#include <video/omapdss.h>

#define ZOOM_NAND_CS    0

extern int __init zoom_debugboard_init(void);
extern void __init zoom_peripherals_init(void);
extern void __init zoom_display_init(void);

#define ZOOM2_HEADSET_EXTMUTE_GPIO	153
