/*
 * arch/arm/mach-davinci/include/mach/mmc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *  Board-specific MMC configuration
 */

#ifndef _DAVINCI_MMC_H
#define _DAVINCI_MMC_H

#include <linux/types.h>
#include <linux/mmc/host.h>

struct davinci_mmc_config {
	/* get_cd()/get_wp() may sleep */
	int	(*get_cd)(int module);
	int	(*get_ro)(int module);

	void	(*set_power)(int module, bool on);

	/* wires == 0 is equivalent to wires == 4 (4-bit parallel) */
	u8	wires;

	u32     max_freq;

	/* any additional host capabilities: OR'd in to mmc->f_caps */
	u32     caps;

	/* Version of the MMC/SD controller */
	u8	version;

	/* Number of sg segments */
	u8	nr_sg;
};
void davinci_setup_mmc(int module, struct davinci_mmc_config *config);

enum {
	MMC_CTLR_VERSION_1 = 0,	/* DM644x and DM355 */
	MMC_CTLR_VERSION_2,	/* DA830 */
};

#endif
