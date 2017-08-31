/*
 * lib/lcm.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/gcd.h>
#include <linux/export.h>
#include <linux/lcm.h>

/* Lowest common multiple */
unsigned long lcm(unsigned long a, unsigned long b)
{
	if (a && b)
		return (a * b) / gcd(a, b);
	else if (b)
		return b;

	return a;
}
EXPORT_SYMBOL_GPL(lcm);
