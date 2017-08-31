/*
 * arch/parisc/include/asm/shmparam.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASMPARISC_SHMPARAM_H
#define _ASMPARISC_SHMPARAM_H

#define __ARCH_FORCE_SHMLBA 	1

#define SHMLBA 0x00400000   /* attach addr needs to be 4 Mb aligned */

#endif /* _ASMPARISC_SHMPARAM_H */
