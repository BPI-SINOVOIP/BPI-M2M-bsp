/*
 * tools/perf/util/debugfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __DEBUGFS_H__
#define __DEBUGFS_H__

const char *debugfs_find_mountpoint(void);
int debugfs_valid_mountpoint(const char *debugfs);
char *debugfs_mount(const char *mountpoint);
void debugfs_set_path(const char *mountpoint);

extern char debugfs_mountpoint[];
extern char tracing_events_path[];

#endif /* __DEBUGFS_H__ */
