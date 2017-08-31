/*
 * arch/tile/include/asm/stat.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#if defined(__KERNEL__) && defined(CONFIG_COMPAT)
#define __ARCH_WANT_STAT64	/* Used for compat_sys_stat64() etc. */
#endif
#include <asm-generic/stat.h>
