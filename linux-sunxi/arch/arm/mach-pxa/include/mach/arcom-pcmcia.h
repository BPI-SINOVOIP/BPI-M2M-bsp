/*
 * arch/arm/mach-pxa/include/mach/arcom-pcmcia.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ARCOM_PCMCIA_H
#define __ARCOM_PCMCIA_H

struct arcom_pcmcia_pdata {
	int	cd_gpio;
	int	rdy_gpio;
	int	pwr_gpio;
	void	(*reset)(int state);
};

#endif
