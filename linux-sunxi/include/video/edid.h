/*
 * include/video/edid.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __linux_video_edid_h__
#define __linux_video_edid_h__

struct edid_info {
	unsigned char dummy[128];
};

#ifdef __KERNEL__
#ifdef CONFIG_X86
extern struct edid_info edid_info;
#endif
#endif

#endif /* __linux_video_edid_h__ */
