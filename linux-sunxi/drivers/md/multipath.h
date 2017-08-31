/*
 * drivers/md/multipath.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _MULTIPATH_H
#define _MULTIPATH_H

struct multipath_info {
	struct md_rdev	*rdev;
};

struct mpconf {
	struct mddev			*mddev;
	struct multipath_info	*multipaths;
	int			raid_disks;
	spinlock_t		device_lock;
	struct list_head	retry_list;

	mempool_t		*pool;
};

/*
 * this is our 'private' 'collective' MULTIPATH buffer head.
 * it contains information about what kind of IO operations were started
 * for this MULTIPATH operation, and about their status:
 */

struct multipath_bh {
	struct mddev			*mddev;
	struct bio		*master_bio;
	struct bio		bio;
	int			path;
	struct list_head	retry_list;
};
#endif
