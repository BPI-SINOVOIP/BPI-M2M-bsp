/*
 * arch/um/include/asm/dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_DMA_H
#define __UM_DMA_H

#include "asm/io.h"

extern unsigned long uml_physmem;

#define MAX_DMA_ADDRESS (uml_physmem)

#endif
