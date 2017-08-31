/*
 * arch/s390/include/asm/isc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_S390_ISC_H
#define _ASM_S390_ISC_H

#include <linux/types.h>

/*
 * I/O interruption subclasses used by drivers.
 * Please add all used iscs here so that it is possible to distribute
 * isc usage between drivers.
 * Reminder: 0 is highest priority, 7 lowest.
 */
#define MAX_ISC 7

/* Regular I/O interrupts. */
#define IO_SCH_ISC 3			/* regular I/O subchannels */
#define CONSOLE_ISC 1			/* console I/O subchannel */
#define CHSC_SCH_ISC 7			/* CHSC subchannels */
/* Adapter interrupts. */
#define QDIO_AIRQ_ISC IO_SCH_ISC	/* I/O subchannel in qdio mode */
#define AP_ISC 6			/* adjunct processor (crypto) devices */

/* Functions for registration of I/O interruption subclasses */
void isc_register(unsigned int isc);
void isc_unregister(unsigned int isc);

#endif /* _ASM_S390_ISC_H */
