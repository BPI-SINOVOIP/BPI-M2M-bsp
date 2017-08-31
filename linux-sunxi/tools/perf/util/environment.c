/*
 * tools/perf/util/environment.c
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
 * We put all the perf config variables in this same object
 * file, so that programs can link against the config parser
 * without having to link against all the rest of perf.
 */
#include "cache.h"

const char *pager_program;
int pager_use_color = 1;
