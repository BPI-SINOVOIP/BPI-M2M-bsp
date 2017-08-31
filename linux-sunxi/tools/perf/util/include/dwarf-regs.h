/*
 * tools/perf/util/include/dwarf-regs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_DWARF_REGS_H_
#define _PERF_DWARF_REGS_H_

#ifdef DWARF_SUPPORT
const char *get_arch_regstr(unsigned int n);
#endif

#endif
