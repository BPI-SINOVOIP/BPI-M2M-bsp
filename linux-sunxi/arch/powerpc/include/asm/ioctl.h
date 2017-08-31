/*
 * arch/powerpc/include/asm/ioctl.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_POWERPC_IOCTL_H
#define _ASM_POWERPC_IOCTL_H

#define _IOC_SIZEBITS	13
#define _IOC_DIRBITS	3

#define _IOC_NONE	1U
#define _IOC_READ	2U
#define _IOC_WRITE	4U

#include <asm-generic/ioctl.h>

#endif	/* _ASM_POWERPC_IOCTL_H */
