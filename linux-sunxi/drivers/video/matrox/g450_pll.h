/*
 * drivers/video/matrox/g450_pll.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __G450_PLL_H__
#define __G450_PLL_H__

#include "matroxfb_base.h"

int matroxfb_g450_setclk(struct matrox_fb_info *minfo, unsigned int fout,
			 unsigned int pll);
unsigned int g450_mnp2f(const struct matrox_fb_info *minfo, unsigned int mnp);
void matroxfb_g450_setpll_cond(struct matrox_fb_info *minfo, unsigned int mnp,
			       unsigned int pll);

#endif	/* __G450_PLL_H__ */
