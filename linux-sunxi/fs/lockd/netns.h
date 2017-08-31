/*
 * fs/lockd/netns.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LOCKD_NETNS_H__
#define __LOCKD_NETNS_H__

#include <net/netns/generic.h>

struct lockd_net {
	unsigned int nlmsvc_users;
};

extern int lockd_net_id;

#endif
