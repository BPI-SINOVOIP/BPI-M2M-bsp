/*
 * arch/microblaze/include/asm/mmu_context.h
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
# include "mmu_context_mm.h"
#else
# include <asm-generic/mmu_context.h>
#endif
