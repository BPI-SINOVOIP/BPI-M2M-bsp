/*
 * tools/perf/util/ui/keysyms.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _PERF_KEYSYMS_H_
#define _PERF_KEYSYMS_H_ 1

#include "libslang.h"

#define K_DOWN	SL_KEY_DOWN
#define K_END	SL_KEY_END
#define K_ENTER	'\r'
#define K_ESC	033
#define K_F1	SL_KEY_F(1)
#define K_HOME	SL_KEY_HOME
#define K_LEFT	SL_KEY_LEFT
#define K_PGDN	SL_KEY_NPAGE
#define K_PGUP	SL_KEY_PPAGE
#define K_RIGHT	SL_KEY_RIGHT
#define K_TAB	'\t'
#define K_UNTAB	SL_KEY_UNTAB
#define K_UP	SL_KEY_UP
#define K_BKSPC 0x7f
#define K_DEL	SL_KEY_DELETE

/* Not really keys */
#define K_TIMER	 -1
#define K_ERROR	 -2
#define K_RESIZE -3

#endif /* _PERF_KEYSYMS_H_ */
