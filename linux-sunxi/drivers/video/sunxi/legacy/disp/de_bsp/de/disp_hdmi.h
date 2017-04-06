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

#ifndef __DISP_HDMI_H__
#define __DISP_HDMI_H__

#include "disp_display_i.h"

__s32 disp_hdmi_init(void);
__s32 disp_hdmi_exit(void);
__s32 disp_hdmi_get_support(__u32 screen_id);


#endif
