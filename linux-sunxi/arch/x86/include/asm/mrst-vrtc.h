/*
 * arch/x86/include/asm/mrst-vrtc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _MRST_VRTC_H
#define _MRST_VRTC_H

extern unsigned char vrtc_cmos_read(unsigned char reg);
extern void vrtc_cmos_write(unsigned char val, unsigned char reg);
extern unsigned long vrtc_get_time(void);
extern int vrtc_set_mmss(unsigned long nowtime);

#endif
