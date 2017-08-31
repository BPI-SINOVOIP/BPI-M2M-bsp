/*
 * net/compat-wireless/include/linux/rfkill.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __COMPAT_RFKILL_H
#define __COMPAT_RFKILL_H

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))

#include_next <linux/rfkill.h>

#else

#include <linux/compat-2.6.h>

#undef CONFIG_RFKILL
#undef CONFIG_RFKILL_INPUT
#undef CONFIG_RFKILL_LEDS

#ifdef CONFIG_RFKILL_BACKPORT
#define CONFIG_RFKILL 1
#endif

#ifdef CONFIG_RFKILL_BACKPORT_INPUT
#define CONFIG_RFKILL_INPUT
#endif

#ifdef CONFIG_RFKILL_BACKPORT_LEDS
#define CONFIG_RFKILL_LEDS
#endif

#include <linux/rfkill_backport.h>

#endif

#endif
