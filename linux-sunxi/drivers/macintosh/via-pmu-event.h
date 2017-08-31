/*
 * drivers/macintosh/via-pmu-event.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __VIA_PMU_EVENT_H
#define __VIA_PMU_EVENT_H

#define PMU_EVT_POWER	0
#define PMU_EVT_LID	1
extern void via_pmu_event(int key, int down);

#endif /* __VIA_PMU_EVENT_H */
