/*
 * include/linux/regulator/of_regulator.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * OpenFirmware regulator support routines
 *
 */

#ifndef __LINUX_OF_REG_H
#define __LINUX_OF_REG_H

#if defined(CONFIG_OF)
extern struct regulator_init_data
	*of_get_regulator_init_data(struct device *dev,
				    struct device_node *node);
#else
static inline struct regulator_init_data
	*of_get_regulator_init_data(struct device *dev,
				    struct device_node *node)
{
	return NULL;
}
#endif /* CONFIG_OF */

#endif /* __LINUX_OF_REG_H */
