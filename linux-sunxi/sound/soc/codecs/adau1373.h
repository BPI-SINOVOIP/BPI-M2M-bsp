/*
 * sound/soc/codecs/adau1373.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ADAU1373_H__
#define __ADAU1373_H__

enum adau1373_pll_src {
	ADAU1373_PLL_SRC_MCLK1 = 0,
	ADAU1373_PLL_SRC_BCLK1 = 1,
	ADAU1373_PLL_SRC_BCLK2 = 2,
	ADAU1373_PLL_SRC_BCLK3 = 3,
	ADAU1373_PLL_SRC_LRCLK1 = 4,
	ADAU1373_PLL_SRC_LRCLK2 = 5,
	ADAU1373_PLL_SRC_LRCLK3 = 6,
	ADAU1373_PLL_SRC_GPIO1 = 7,
	ADAU1373_PLL_SRC_GPIO2 = 8,
	ADAU1373_PLL_SRC_GPIO3 = 9,
	ADAU1373_PLL_SRC_GPIO4 = 10,
	ADAU1373_PLL_SRC_MCLK2 = 11,
};

enum adau1373_pll {
	ADAU1373_PLL1 = 0,
	ADAU1373_PLL2 = 1,
};

enum adau1373_clk_src {
	ADAU1373_CLK_SRC_PLL1 = 0,
	ADAU1373_CLK_SRC_PLL2 = 1,
};

#endif
