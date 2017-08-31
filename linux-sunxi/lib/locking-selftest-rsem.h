/*
 * lib/locking-selftest-rsem.h
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
#define LOCK		RSL

#undef UNLOCK
#define UNLOCK		RSU

#undef RLOCK
#define RLOCK		RSL

#undef WLOCK
#define WLOCK		WSL

#undef INIT
#define INIT		RWSI
