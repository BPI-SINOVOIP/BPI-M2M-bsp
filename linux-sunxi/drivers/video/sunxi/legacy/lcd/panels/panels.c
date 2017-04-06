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
#include "lp079x01.h"

extern __lcd_panel_t lp079x01_panel;
extern __lcd_panel_t default_panel;

__lcd_panel_t* panel_array[] = {
	&default_panel,
	&lp079x01_panel,
	/* add new panel below */

	NULL,
};

