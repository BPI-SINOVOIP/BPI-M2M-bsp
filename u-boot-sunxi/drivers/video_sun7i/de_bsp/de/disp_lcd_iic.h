/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "disp_display_i.h"
#include "disp_lcd.h"
#include "disp_display.h"

//---------------------------------------------------------
__s32 bsp_disp_lcd_iic_write(__u8 slave_addr, __u8 sub_addr, __u8 value);
__s32 bsp_disp_lcd_iic_read(__u8 slave_addr, __u8 sub_addr, __u8* value);

