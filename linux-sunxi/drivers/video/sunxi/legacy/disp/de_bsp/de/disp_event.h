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

#ifndef __DISP_EVENT_H__
#define __DISP_EVENT_H__

#include "disp_display_i.h"
#include "disp_layer.h"


void LCD_vbi_event_proc(__u32 screen_id, __u32 tcon_index);
void LCD_line_event_proc(__u32 screen_id);
__s32 bsp_disp_cfg_start(__u32 screen_id);
__s32 bsp_disp_cfg_finish(__u32 screen_id);

#endif
