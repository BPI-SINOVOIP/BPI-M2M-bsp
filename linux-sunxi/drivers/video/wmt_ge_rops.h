/*
 * drivers/video/wmt_ge_rops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
extern void wmt_ge_fillrect(struct fb_info *info,
			    const struct fb_fillrect *rect);
extern void wmt_ge_copyarea(struct fb_info *info,
			    const struct fb_copyarea *area);
extern int wmt_ge_sync(struct fb_info *info);
