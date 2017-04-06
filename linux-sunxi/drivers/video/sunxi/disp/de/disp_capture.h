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

#ifndef __DISP_CAPTURE_H__
#define __DISP_CAPTURE_H__

#include "disp_private.h"

s32 disp_init_capture(__disp_bsp_init_para *para);
extern s32 disp_al_capture_init(u32 screen_id);
extern s32 disp_al_capture_sync(u32 screen_id);

#endif
