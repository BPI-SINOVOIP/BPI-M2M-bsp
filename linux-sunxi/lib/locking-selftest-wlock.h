/*
 * lib/locking-selftest-wlock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#undef LOCK
#define LOCK		WL

#undef UNLOCK
#define UNLOCK		WU

#undef RLOCK
#define RLOCK		RL

#undef WLOCK
#define WLOCK		WL

#undef INIT
#define INIT		RWI
