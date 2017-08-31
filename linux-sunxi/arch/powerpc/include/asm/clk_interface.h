/*
 * arch/powerpc/include/asm/clk_interface.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_POWERPC_CLK_INTERFACE_H
#define __ASM_POWERPC_CLK_INTERFACE_H

#include <linux/clk.h>

struct clk_interface {
	struct clk*	(*clk_get)	(struct device *dev, const char *id);
	int		(*clk_enable)	(struct clk *clk);
	void		(*clk_disable)	(struct clk *clk);
	unsigned long	(*clk_get_rate)	(struct clk *clk);
	void		(*clk_put)	(struct clk *clk);
	long		(*clk_round_rate) (struct clk *clk, unsigned long rate);
	int 		(*clk_set_rate)	(struct clk *clk, unsigned long rate);
	int		(*clk_set_parent) (struct clk *clk, struct clk *parent);
	struct clk*	(*clk_get_parent) (struct clk *clk);
};

extern struct clk_interface clk_functions;

#endif /* __ASM_POWERPC_CLK_INTERFACE_H */
