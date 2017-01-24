/*
 *
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
#include "B116XAN03.h"
#include "gm7121_cvbs.h"

extern __lcd_panel_t tft720x1280_panel;
extern __lcd_panel_t vvx10f004b00_panel;
extern __lcd_panel_t lp907qx_panel;
extern __lcd_panel_t starry768x1024_panel;
extern __lcd_panel_t sl698ph_720p_panel;
extern __lcd_panel_t B116XAN03_panel;
extern __lcd_panel_t gm7121_cvbs;

__lcd_panel_t* panel_array[] = {
#if defined(CONFIG_TV_GM7121)
	&gm7121_cvbs,
#endif
	&default_panel,
	&tft720x1280_panel,
	&vvx10f004b00_panel,
	&lp907qx_panel,
	&starry768x1024_panel,
	&sl698ph_720p_panel,
	&B116XAN03_panel,
	/* add new panel below */

	NULL,
};

