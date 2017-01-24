/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "panels.h"
#include "default_panel.h"
#include "tft720x1280.h"
#include "lp907qx.h"
#include "starry768x1024.h"
#include "B116XAN03.h"

extern __lcd_panel_t sl698ph_720p_panel;
extern __lcd_panel_t lp079x01_panel;

__lcd_panel_t* panel_array[] = {
	&default_panel,
	&tft720x1280_panel,
	&lp907qx_panel,
	&starry768x1024_panel,
	&sl698ph_720p_panel,
	&lp079x01_panel,
	&B116XAN03_panel,
	/* add new panel below */

	NULL,
};

