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
#ifndef __PANEL_H__
#define __PANEL_H__
#include "../lcd_source_interface.h"
#include "../lcd_panel_cfg.h"

typedef struct
{
	char name[32];
	__lcd_panel_fun_t func;
}__lcd_panel_t;

extern __lcd_panel_t * panel_array[];

#endif