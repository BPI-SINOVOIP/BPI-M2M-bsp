/*
 * drivers/staging/bcm/DDRInit.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _DDR_INIT_H_
#define _DDR_INIT_H_



int ddr_init(PMINI_ADAPTER	psAdapter);
int download_ddr_settings(PMINI_ADAPTER	psAdapter);

#endif
