/*
 * arch/sh/boot/compressed/cache.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
int cache_control(unsigned int command)
{
	volatile unsigned int *p = (volatile unsigned int *) 0x80000000;
	int i;

	for (i = 0; i < (32 * 1024); i += 32) {
		(void)*p;
		p += (32 / sizeof(int));
	}

	return 0;
}
