/*
 * tools/perf/util/xyarray.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "xyarray.h"
#include "util.h"

struct xyarray *xyarray__new(int xlen, int ylen, size_t entry_size)
{
	size_t row_size = ylen * entry_size;
	struct xyarray *xy = zalloc(sizeof(*xy) + xlen * row_size);

	if (xy != NULL) {
		xy->entry_size = entry_size;
		xy->row_size   = row_size;
	}

	return xy;
}

void xyarray__delete(struct xyarray *xy)
{
	free(xy);
}
