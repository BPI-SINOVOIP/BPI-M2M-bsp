/*
 * include/linux/crush/mapper.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H

/*
 * CRUSH functions for find rules and then mapping an input to an
 * output set.
 *
 * LGPL2
 */

#include "crush.h"

extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 int forcefeed,    /* -1 for none */
			 const __u32 *weights);

#endif
