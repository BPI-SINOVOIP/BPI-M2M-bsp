/*
 * arch/m68k/kernel/dma.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_MMU
#include "dma_mm.c"
#else
#include "dma_no.c"
#endif
