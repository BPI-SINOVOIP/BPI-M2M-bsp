/*
 * drivers/power/axp_power/axp20/axp20-regu.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_AXP20_REGU_H_
#define _LINUX_AXP20_REGU_H_


/* AXP20 Regulator Registers */
#define AXP20_LDO1		POWER20_STATUS
#define AXP20_LDO2		POWER20_LDO24OUT_VOL
#define AXP20_LDO3		POWER20_LDO3OUT_VOL
#define AXP20_LDO4		POWER20_LDO24OUT_VOL
#define AXP20_BUCK2		POWER20_DC2OUT_VOL
#define AXP20_BUCK3		POWER20_DC3OUT_VOL
#define AXP20_LDOIO0		POWER20_GPIO0_VOL

#define AXP20_LDO1EN		POWER20_STATUS
#define AXP20_LDO2EN		POWER20_LDO234_DC23_CTL
#define AXP20_LDO3EN		POWER20_LDO234_DC23_CTL
#define AXP20_LDO4EN		POWER20_LDO234_DC23_CTL
#define AXP20_BUCK2EN		POWER20_LDO234_DC23_CTL
#define AXP20_BUCK3EN		POWER20_LDO234_DC23_CTL
#define AXP20_LDOIOEN		POWER20_GPIO0_CTL


#define AXP20_BUCKMODE		POWER20_DCDC_MODESET
#define AXP20_BUCKFREQ		POWER20_DCDC_FREQSET


extern struct axp_funcdev_info *axp20_regu_init(void);


#endif

