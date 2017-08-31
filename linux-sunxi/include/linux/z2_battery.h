/*
 * include/linux/z2_battery.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_Z2_BATTERY_H
#define _LINUX_Z2_BATTERY_H

struct z2_battery_info {
	int	 batt_I2C_bus;
	int	 batt_I2C_addr;
	int	 batt_I2C_reg;
	int	 charge_gpio;
	int	 min_voltage;
	int	 max_voltage;
	int	 batt_div;
	int	 batt_mult;
	int	 batt_tech;
	char	*batt_name;
};

#endif
