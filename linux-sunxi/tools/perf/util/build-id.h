/*
 * tools/perf/util/build-id.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef PERF_BUILD_ID_H_
#define PERF_BUILD_ID_H_ 1

#include "session.h"

extern struct perf_tool build_id__mark_dso_hit_ops;

char *dso__build_id_filename(struct dso *self, char *bf, size_t size);

#endif
