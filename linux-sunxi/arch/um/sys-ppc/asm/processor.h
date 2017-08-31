/*
 * arch/um/sys-ppc/asm/processor.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_PROCESSOR_PPC_H
#define __UM_PROCESSOR_PPC_H

#if defined(__ASSEMBLY__)

#define CONFIG_PPC_MULTIPLATFORM
#include "arch/processor.h"

#else

#include "asm/processor-generic.h"

#endif

#endif
