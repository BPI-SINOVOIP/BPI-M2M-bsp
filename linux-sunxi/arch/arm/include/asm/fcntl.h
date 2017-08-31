/*
 * arch/arm/include/asm/fcntl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ARM_FCNTL_H
#define _ARM_FCNTL_H

#define O_DIRECTORY	 040000	/* must be a directory */
#define O_NOFOLLOW	0100000	/* don't follow links */
#define O_DIRECT	0200000	/* direct disk access hint - currently ignored */
#define O_LARGEFILE	0400000

#include <asm-generic/fcntl.h>

#endif
