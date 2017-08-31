/*
 * arch/um/drivers/slip.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_SLIP_H
#define __UM_SLIP_H

#include "slip_common.h"

struct slip_data {
	void *dev;
	char name[sizeof("slnnnnn\0")];
	char *addr;
	char *gate_addr;
	int slave;
	struct slip_proto slip;
};

extern const struct net_user_info slip_user_info;

extern int slip_user_read(int fd, void *buf, int len, struct slip_data *pri);
extern int slip_user_write(int fd, void *buf, int len, struct slip_data *pri);

#endif
