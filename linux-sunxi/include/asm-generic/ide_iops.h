/*
 * include/asm-generic/ide_iops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Generic I/O and MEMIO string operations.  */

#define __ide_insw	insw
#define __ide_insl	insl
#define __ide_outsw	outsw
#define __ide_outsl	outsl

static __inline__ void __ide_mm_insw(void __iomem *port, void *addr, u32 count)
{
	while (count--) {
		*(u16 *)addr = readw(port);
		addr += 2;
	}
}

static __inline__ void __ide_mm_insl(void __iomem *port, void *addr, u32 count)
{
	while (count--) {
		*(u32 *)addr = readl(port);
		addr += 4;
	}
}

static __inline__ void __ide_mm_outsw(void __iomem *port, void *addr, u32 count)
{
	while (count--) {
		writew(*(u16 *)addr, port);
		addr += 2;
	}
}

static __inline__ void __ide_mm_outsl(void __iomem * port, void *addr, u32 count)
{
	while (count--) {
		writel(*(u32 *)addr, port);
		addr += 4;
	}
}
