/*
 * block/partitions/osf.h
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
 *  fs/partitions/osf.h
 */

#define DISKLABELMAGIC (0x82564557UL)

int osf_partition(struct parsed_partitions *state);
