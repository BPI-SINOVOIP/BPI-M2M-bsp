/*
 * arch/x86/kernel/cpu/umc.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/processor.h>
#include "cpu.h"

/*
 * UMC chips appear to be only either 386 or 486,
 * so no special init takes place.
 */

static const struct cpu_dev __cpuinitconst umc_cpu_dev = {
	.c_vendor	= "UMC",
	.c_ident	= { "UMC UMC UMC" },
	.c_models = {
		{ .vendor = X86_VENDOR_UMC, .family = 4, .model_names =
		  {
			  [1] = "U5D",
			  [2] = "U5S",
		  }
		},
	},
	.c_x86_vendor	= X86_VENDOR_UMC,
};

cpu_dev_register(umc_cpu_dev);

