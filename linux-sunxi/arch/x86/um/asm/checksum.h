/*
 * arch/x86/um/asm/checksum.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_CHECKSUM_H
#define __UM_CHECKSUM_H

#ifdef CONFIG_X86_32
# include "checksum_32.h"
#else
# include "checksum_64.h"
#endif

#endif
