/*
 * arch/powerpc/include/asm/mpc8xx.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* This is the single file included by all MPC8xx build options.
 * Since there are many different boards and no standard configuration,
 * we have a unique include file for each.  Rather than change every
 * file that has to include MPC8xx configuration, they all include
 * this one and the configuration switching is done here.
 */
#ifndef __CONFIG_8xx_DEFS
#define __CONFIG_8xx_DEFS

extern struct mpc8xx_pcmcia_ops m8xx_pcmcia_ops;

#endif /* __CONFIG_8xx_DEFS */
