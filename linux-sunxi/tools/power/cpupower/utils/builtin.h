/*
 * tools/power/cpupower/utils/builtin.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef BUILTIN_H
#define BUILTIN_H

extern int cmd_set(int argc, const char **argv);
extern int cmd_info(int argc, const char **argv);
extern int cmd_freq_set(int argc, const char **argv);
extern int cmd_freq_info(int argc, const char **argv);
extern int cmd_idle_info(int argc, const char **argv);
extern int cmd_monitor(int argc, const char **argv);

#endif
