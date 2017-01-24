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
#ifndef _DISP_SMCL_H_
#define _DISP_SMCL_H_
#include "bsp_display.h"
#include "disp_private.h"

s32 disp_smcl_shadow_protect(struct disp_smcl *smcl, bool protect);
s32 disp_init_smcl(__disp_bsp_init_para * para);
s32 disp_smcl_set_size(struct disp_smcl* smcl, disp_size *size);

#endif

