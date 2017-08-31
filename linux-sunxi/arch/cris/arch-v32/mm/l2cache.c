/*
 * arch/cris/arch-v32/mm/l2cache.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <memmap.h>
#include <hwregs/reg_map.h>
#include <hwregs/reg_rdwr.h>
#include <hwregs/l2cache_defs.h>
#include <asm/io.h>

#define L2CACHE_SIZE 64

int __init l2cache_init(void)
{
	reg_l2cache_rw_ctrl ctrl = {0};
	reg_l2cache_rw_cfg cfg = {.en = regk_l2cache_yes};

	ctrl.csize = L2CACHE_SIZE;
	ctrl.cbase = L2CACHE_SIZE / 4 + (L2CACHE_SIZE % 4 ? 1 : 0);
	REG_WR(l2cache, regi_l2cache, rw_ctrl, ctrl);

	/* Flush the tag memory */
	memset((void *)(MEM_INTMEM_START | MEM_NON_CACHEABLE), 0, 2*1024);

	/* Enable the cache */
	REG_WR(l2cache, regi_l2cache, rw_cfg, cfg);

	return 0;
}

