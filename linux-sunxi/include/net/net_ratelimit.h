/*
 * include/net/net_ratelimit.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_NET_RATELIMIT_H
#define _LINUX_NET_RATELIMIT_H

#include <linux/ratelimit.h>

extern struct ratelimit_state net_ratelimit_state;

#endif	/* _LINUX_NET_RATELIMIT_H */
