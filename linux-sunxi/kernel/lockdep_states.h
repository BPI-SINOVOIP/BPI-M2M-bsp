/*
 * kernel/lockdep_states.h
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
 * Lockdep states,
 *
 * please update XXX_LOCK_USAGE_STATES in include/linux/lockdep.h whenever
 * you add one, or come up with a nice dynamic solution.
 */
LOCKDEP_STATE(HARDIRQ)
LOCKDEP_STATE(SOFTIRQ)
LOCKDEP_STATE(RECLAIM_FS)
