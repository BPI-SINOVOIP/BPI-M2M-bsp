/*
 * arch/sh/include/cpu-sh5/cpu/rtc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_CPU_SH5_RTC_H
#define __ASM_SH_CPU_SH5_RTC_H

#define rtc_reg_size		sizeof(u32)
#define RTC_BIT_INVERTED	0	/* The SH-5 RTC is surprisingly sane! */
#define RTC_DEF_CAPABILITIES	RTC_CAP_4_DIGIT_YEAR

#endif /* __ASM_SH_CPU_SH5_RTC_H */
