/*
 * lib/locking-selftest-mutex.h
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
#define LOCK		ML

#undef UNLOCK
#define UNLOCK		MU

#undef RLOCK
#undef WLOCK

#undef INIT
#define INIT		MI
