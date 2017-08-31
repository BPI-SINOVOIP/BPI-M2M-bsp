/*
 * arch/arm/kernel/atags.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_ATAGS_PROC
extern void save_atags(struct tag *tags);
#else
static inline void save_atags(struct tag *tags) { }
#endif
