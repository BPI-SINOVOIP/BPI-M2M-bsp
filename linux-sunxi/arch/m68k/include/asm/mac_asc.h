/*
 * arch/m68k/include/asm/mac_asc.h
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
 *	Apple Sound Chip
 */

#ifndef __ASM_MAC_ASC_H
#define __ASM_MAC_ASC_H

/*
 *	ASC offsets and controls
 */

#define ASC_BUF_BASE	0x00	/* RAM buffer offset */
#define ASC_BUF_SIZE	0x800

#define ASC_CONTROL	0x800
#define ASC_CONTROL_OFF		0x00
#define ASC_FREQ(chan,byte)	((0x810)+((chan)<<3)+(byte))
#define ASC_ENABLE	0x801
#define ASC_ENABLE_SAMPLE	0x02
#define ASC_MODE	0x802
#define ASC_MODE_SAMPLE		0x02

#define ASC_VOLUME	0x806
#define ASC_CHAN	0x807	/* ??? */


#endif
