/*
 * lib/locking-selftest-hardirq.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#undef IRQ_DISABLE
#undef IRQ_ENABLE
#undef IRQ_ENTER
#undef IRQ_EXIT

#define IRQ_ENABLE		HARDIRQ_ENABLE
#define IRQ_DISABLE		HARDIRQ_DISABLE
#define IRQ_ENTER		HARDIRQ_ENTER
#define IRQ_EXIT		HARDIRQ_EXIT
