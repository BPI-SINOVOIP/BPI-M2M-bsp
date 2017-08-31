/*
 * include/video/omap-panel-n8x0.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __OMAP_PANEL_N8X0_H
#define __OMAP_PANEL_N8X0_H

struct omap_dss_device;

struct panel_n8x0_data {
	int (*platform_enable)(struct omap_dss_device *dssdev);
	void (*platform_disable)(struct omap_dss_device *dssdev);
	int panel_reset;
	int ctrl_pwrdown;

	int (*set_backlight)(struct omap_dss_device *dssdev, int level);
};

#endif
