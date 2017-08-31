/*
 * arch/arm/include/asm/statfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASMARM_STATFS_H
#define _ASMARM_STATFS_H

/*
 * With EABI there is 4 bytes of padding added to this structure.
 * Let's pack it so the padding goes away to simplify dual ABI support.
 * Note that user space does NOT have to pack this structure.
 */
#define ARCH_PACK_STATFS64 __attribute__((packed,aligned(4)))

#include <asm-generic/statfs.h>
#endif
