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

#ifndef __DISP_DE_H_
#define __DISP_DE_H_

#include "disp_display_i.h"

extern __hdle   h_tvahbclk;
extern __hdle   h_tv1clk;
extern __hdle   h_tv2clk;

#ifdef __LINUX_OSAL__
__s32 scaler_event_proc(int irq, void *parg);
#else
__s32 scaler_event_proc(void *parg);
#endif

__s32 Image_init(__u32 screen_id);
__s32 Image_exit(__u32 screen_id);
__s32 Image_open(__u32 screen_id);
__s32 Image_close(__u32 screen_id);
__s32 Disp_set_out_interlace(__u32 screen_id);

#endif
