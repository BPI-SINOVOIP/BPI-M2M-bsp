/*
 * arch/cris/include/asm/ioctls.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ARCH_CRIS_IOCTLS_H__
#define __ARCH_CRIS_IOCTLS_H__

#define TIOCSERGSTRUCT	0x5458 /* For debugging only */
#define TIOCSERSETRS485	0x5461  /* enable rs-485 (deprecated) */
#define TIOCSERWRRS485	0x5462  /* write rs-485 */
#define TIOCSRS485	0x5463  /* enable rs-485 */

#include <asm-generic/ioctls.h>

#endif
