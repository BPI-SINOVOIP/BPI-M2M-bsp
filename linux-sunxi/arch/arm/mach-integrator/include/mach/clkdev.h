/*
 * arch/arm/mach-integrator/include/mach/clkdev.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MACH_CLKDEV_H
#define __ASM_MACH_CLKDEV_H

#include <linux/module.h>
#include <plat/clock.h>

struct clk {
	unsigned long		rate;
	const struct clk_ops	*ops;
	struct module		*owner;
	const struct icst_params *params;
	void __iomem		*vcoreg;
	void			*data;
};

static inline int __clk_get(struct clk *clk)
{
	return try_module_get(clk->owner);
}

static inline void __clk_put(struct clk *clk)
{
	module_put(clk->owner);
}

#endif
