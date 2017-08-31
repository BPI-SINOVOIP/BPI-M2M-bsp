/*
 * arch/avr32/include/asm/param.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_AVR32_PARAM_H
#define __ASM_AVR32_PARAM_H

#ifdef __KERNEL__
# define HZ		CONFIG_HZ
# define USER_HZ	100		/* User interfaces are in "ticks" */
# define CLOCKS_PER_SEC	(USER_HZ)	/* frequency at which times() counts */
#endif

#ifndef HZ
# define HZ		100
#endif

/* TODO: Should be configurable */
#define EXEC_PAGESIZE	4096

#ifndef NOGROUP
# define NOGROUP	(-1)
#endif

#define MAXHOSTNAMELEN	64

#endif /* __ASM_AVR32_PARAM_H */
