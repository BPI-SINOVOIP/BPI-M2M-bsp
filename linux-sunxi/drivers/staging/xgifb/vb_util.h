/*
 * drivers/staging/xgifb/vb_util.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _VBUTIL_
#define _VBUTIL_
extern void xgifb_reg_set(unsigned long, u8, u8);
extern u8 xgifb_reg_get(unsigned long, u8);
extern void xgifb_reg_or(unsigned long, u8, unsigned);
extern void xgifb_reg_and(unsigned long, u8, unsigned);
extern void xgifb_reg_and_or(unsigned long, u8, unsigned, unsigned);
#endif

