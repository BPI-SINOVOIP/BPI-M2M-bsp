/*
 * arch/sparc/include/asm/poll.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SPARC_POLL_H
#define __SPARC_POLL_H

#define POLLWRNORM	POLLOUT
#define POLLWRBAND	256
#define POLLMSG		512
#define POLLREMOVE	1024
#define POLLRDHUP       2048

#include <asm-generic/poll.h>

#endif
