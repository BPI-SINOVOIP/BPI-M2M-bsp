/*
 * arch/m68k/include/asm/mc146818rtc.h
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
 * Machine dependent access functions for RTC registers.
 */
#ifndef _ASM_MC146818RTC_H
#define _ASM_MC146818RTC_H


#ifdef CONFIG_ATARI
/* RTC in Atari machines */

#include <asm/atarihw.h>

#define RTC_PORT(x)	(TT_RTC_BAS + 2*(x))
#define RTC_ALWAYS_BCD	0

#define CMOS_READ(addr) ({ \
atari_outb_p((addr),RTC_PORT(0)); \
atari_inb_p(RTC_PORT(1)); \
})
#define CMOS_WRITE(val, addr) ({ \
atari_outb_p((addr),RTC_PORT(0)); \
atari_outb_p((val),RTC_PORT(1)); \
})
#endif /* CONFIG_ATARI */

#endif /* _ASM_MC146818RTC_H */
