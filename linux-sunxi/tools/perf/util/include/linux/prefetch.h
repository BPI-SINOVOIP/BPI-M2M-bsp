/*
 * tools/perf/util/include/linux/prefetch.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef PERF_LINUX_PREFETCH_H
#define PERF_LINUX_PREFETCH_H

static inline void prefetch(void *a __attribute__((unused))) { }

#endif
