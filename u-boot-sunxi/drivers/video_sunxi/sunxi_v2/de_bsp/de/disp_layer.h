/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */


#ifndef __DISP_LAYER_H__
#define __DISP_LAYER_H__

#include "disp_private.h"

s32 disp_init_lyr(__disp_bsp_init_para * para);
s32 disp_lyr_shadow_protect(struct disp_layer *lyr, bool protect);

#endif
