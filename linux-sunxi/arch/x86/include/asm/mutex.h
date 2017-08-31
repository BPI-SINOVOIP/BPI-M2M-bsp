/*
 * arch/x86/include/asm/mutex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_X86_32
# include "mutex_32.h"
#else
# include "mutex_64.h"
#endif
