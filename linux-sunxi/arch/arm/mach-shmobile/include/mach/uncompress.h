/*
 * arch/arm/mach-shmobile/include/mach/uncompress.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MACH_UNCOMPRESS_H
#define __ASM_MACH_UNCOMPRESS_H

/*
 * This does not append a newline
 */
static void putc(int c)
{
}

static inline void flush(void)
{
}

static void arch_decomp_setup(void)
{
}

#define arch_decomp_wdog()

#endif /* __ASM_MACH_UNCOMPRESS_H */
