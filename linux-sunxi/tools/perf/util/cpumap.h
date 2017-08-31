/*
 * tools/perf/util/cpumap.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __PERF_CPUMAP_H
#define __PERF_CPUMAP_H

#include <stdio.h>

struct cpu_map {
	int nr;
	int map[];
};

struct cpu_map *cpu_map__new(const char *cpu_list);
struct cpu_map *cpu_map__dummy_new(void);
void cpu_map__delete(struct cpu_map *map);

size_t cpu_map__fprintf(struct cpu_map *map, FILE *fp);

#endif /* __PERF_CPUMAP_H */
