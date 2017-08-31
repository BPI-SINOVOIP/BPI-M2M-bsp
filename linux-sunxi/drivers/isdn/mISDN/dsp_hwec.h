/*
 * drivers/isdn/mISDN/dsp_hwec.h
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
 * dsp_hwec.h
 */

extern struct mISDN_dsp_element *dsp_hwec;
extern void dsp_hwec_enable(struct dsp *dsp, const char *arg);
extern void dsp_hwec_disable(struct dsp *dsp);
extern int  dsp_hwec_init(void);
extern void dsp_hwec_exit(void);
