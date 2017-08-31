/*
 * tools/perf/util/ui/browsers/map.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_UI_MAP_BROWSER_H_
#define _PERF_UI_MAP_BROWSER_H_ 1
struct map;

int map__browse(struct map *self);
#endif /* _PERF_UI_MAP_BROWSER_H_ */
