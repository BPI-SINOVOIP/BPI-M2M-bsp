/*
 * tools/perf/util/include/asm/hweight.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef PERF_HWEIGHT_H
#define PERF_HWEIGHT_H

#include <linux/types.h>
unsigned int hweight32(unsigned int w);
unsigned long hweight64(__u64 w);

#endif /* PERF_HWEIGHT_H */
