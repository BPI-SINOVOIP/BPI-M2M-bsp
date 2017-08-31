/*
 * arch/alpha/lib/srm_puts.c
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
 *	arch/alpha/lib/srm_puts.c
 */

#include <linux/string.h>
#include <asm/console.h>

long
srm_puts(const char *str, long len)
{
	long remaining, written;

	if (!callback_init_done)
		return len;

	for (remaining = len; remaining > 0; remaining -= written)
	{
		written = callback_puts(0, str, remaining);
		written &= 0xffffffff;
		str += written;
	}
	return len;
}
