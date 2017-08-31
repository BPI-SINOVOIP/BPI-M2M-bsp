/*
 * include/linux/regulator/userspace-consumer.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __REGULATOR_PLATFORM_CONSUMER_H_
#define __REGULATOR_PLATFORM_CONSUMER_H_

struct regulator_consumer_supply;

/**
 * struct regulator_userspace_consumer_data - line consumer
 * initialisation data.
 *
 * @name: Name for the consumer line
 * @num_supplies: Number of supplies feeding the line
 * @supplies: Supplies configuration.
 * @init_on: Set if the regulators supplying the line should be
 *           enabled during initialisation
 */
struct regulator_userspace_consumer_data {
	const char *name;

	int num_supplies;
	struct regulator_bulk_data *supplies;

	bool init_on;
};

#endif /* __REGULATOR_PLATFORM_CONSUMER_H_ */
