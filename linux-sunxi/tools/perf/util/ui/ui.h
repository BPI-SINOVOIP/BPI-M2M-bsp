/*
 * tools/perf/util/ui/ui.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_UI_H_
#define _PERF_UI_H_ 1

#include <pthread.h>
#include <stdbool.h>

extern pthread_mutex_t ui__lock;

void ui__refresh_dimensions(bool force);

#endif /* _PERF_UI_H_ */
