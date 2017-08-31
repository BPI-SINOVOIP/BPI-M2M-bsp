/*
 * net/mac80211/driver-trace.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* bug in tracepoint.h, it should include this */
#include <linux/module.h>

/* sparse isn't too happy with all macros... */
#ifndef __CHECKER__
#include "driver-ops.h"
#define CREATE_TRACE_POINTS
#include "driver-trace.h"
#endif
