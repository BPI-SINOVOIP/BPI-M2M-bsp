/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include "panels.h"
#include "default_panel.h"
#include "tft720x1280.h"
#include "lp907qx.h"
#include "starry768x1024.h"
#include "B116XAN03.h"
#include "LHR050H41_MIPI_RGB.h"
#include "S070WV20_MIPI_RGB.h"

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
	&LHR050H41_MIPI_RGB_panel,
	&S070WV20_MIPI_RGB_panel,
	NULL,
};

