/*
 * include/linux/i2c-pxa.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_I2C_ALGO_PXA_H
#define _LINUX_I2C_ALGO_PXA_H

typedef enum i2c_slave_event_e {
	I2C_SLAVE_EVENT_START_READ,
	I2C_SLAVE_EVENT_START_WRITE,
	I2C_SLAVE_EVENT_STOP
} i2c_slave_event_t;

struct i2c_slave_client {
	void *data;
	void (*event)(void *ptr, i2c_slave_event_t event);
	int  (*read) (void *ptr);
	void (*write)(void *ptr, unsigned int val);
};

#endif /* _LINUX_I2C_ALGO_PXA_H */
