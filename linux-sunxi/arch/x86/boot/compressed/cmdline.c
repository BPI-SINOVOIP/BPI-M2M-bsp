/*
 * arch/x86/boot/compressed/cmdline.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "misc.h"

static unsigned long fs;
static inline void set_fs(unsigned long seg)
{
	fs = seg << 4;  /* shift it back */
}
typedef unsigned long addr_t;
static inline char rdfs8(addr_t addr)
{
	return *((char *)(fs + addr));
}
#include "../cmdline.c"
int cmdline_find_option(const char *option, char *buffer, int bufsize)
{
	return __cmdline_find_option(real_mode->hdr.cmd_line_ptr, option, buffer, bufsize);
}
int cmdline_find_option_bool(const char *option)
{
	return __cmdline_find_option_bool(real_mode->hdr.cmd_line_ptr, option);
}
