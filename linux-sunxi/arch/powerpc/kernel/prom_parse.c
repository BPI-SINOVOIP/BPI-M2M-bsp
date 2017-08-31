/*
 * arch/powerpc/kernel/prom_parse.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#undef DEBUG

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/etherdevice.h>
#include <linux/of_address.h>
#include <asm/prom.h>

void of_parse_dma_window(struct device_node *dn, const void *dma_window_prop,
		unsigned long *busno, unsigned long *phys, unsigned long *size)
{
	const u32 *dma_window;
	u32 cells;
	const unsigned char *prop;

	dma_window = dma_window_prop;

	/* busno is always one cell */
	*busno = *(dma_window++);

	prop = of_get_property(dn, "ibm,#dma-address-cells", NULL);
	if (!prop)
		prop = of_get_property(dn, "#address-cells", NULL);

	cells = prop ? *(u32 *)prop : of_n_addr_cells(dn);
	*phys = of_read_number(dma_window, cells);

	dma_window += cells;

	prop = of_get_property(dn, "ibm,#dma-size-cells", NULL);
	cells = prop ? *(u32 *)prop : of_n_size_cells(dn);
	*size = of_read_number(dma_window, cells);
}
