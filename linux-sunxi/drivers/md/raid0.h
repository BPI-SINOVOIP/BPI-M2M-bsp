/*
 * drivers/md/raid0.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _RAID0_H
#define _RAID0_H

struct strip_zone {
	sector_t zone_end;	/* Start of the next zone (in sectors) */
	sector_t dev_start;	/* Zone offset in real dev (in sectors) */
	int	 nb_dev;	/* # of devices attached to the zone */
};

struct r0conf {
	struct strip_zone	*strip_zone;
	struct md_rdev		**devlist; /* lists of rdevs, pointed to
					    * by strip_zone->dev */
	int			nr_strip_zones;
	int			has_merge_bvec;	/* at least one member has
						 * a merge_bvec_fn */
};

#endif
