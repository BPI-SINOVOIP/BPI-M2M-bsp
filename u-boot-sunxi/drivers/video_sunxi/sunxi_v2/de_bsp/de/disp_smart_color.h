/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef _DISP_SMCL_H_
#define _DISP_SMCL_H_
#include "bsp_display.h"
#include "disp_private.h"

s32 disp_smcl_shadow_protect(struct disp_smcl *smcl, bool protect);
s32 disp_init_smcl(__disp_bsp_init_para * para);

#endif

