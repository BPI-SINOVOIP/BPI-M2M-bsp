/*
 * drivers/scsi/g_NCR5380_mmio.c
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
 *	There is probably a nicer way to do this but this one makes
 *	pretty obvious what is happening. We rebuild the same file with
 *	different options for mmio versus pio.
 */

#define SCSI_G_NCR5380_MEM

#include "g_NCR5380.c"

