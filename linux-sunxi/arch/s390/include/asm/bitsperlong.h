/*
 * arch/s390/include/asm/bitsperlong.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_S390_BITSPERLONG_H
#define __ASM_S390_BITSPERLONG_H

#ifndef __s390x__
#define __BITS_PER_LONG 32
#else
#define __BITS_PER_LONG 64
#endif

#include <asm-generic/bitsperlong.h>

#endif /* __ASM_S390_BITSPERLONG_H */

