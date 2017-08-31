/*
 * include/linux/sh_timer.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SH_TIMER_H__
#define __SH_TIMER_H__

struct sh_timer_config {
	char *name;
	long channel_offset;
	int timer_bit;
	unsigned long clockevent_rating;
	unsigned long clocksource_rating;
};

#endif /* __SH_TIMER_H__ */
