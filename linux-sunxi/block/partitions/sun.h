/*
 * block/partitions/sun.h
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
 *  fs/partitions/sun.h
 */

#define SUN_LABEL_MAGIC          0xDABE
#define SUN_VTOC_SANITY          0x600DDEEE

int sun_partition(struct parsed_partitions *state);
