/*
 * arch/m68k/sun3/leds.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/contregs.h>
#include <asm/sun3mmu.h>
#include <asm/io.h>

void sun3_leds(unsigned char byte)
{
	unsigned char dfc;

	GET_DFC(dfc);
	SET_DFC(FC_CONTROL);
	SET_CONTROL_BYTE(AC_LEDS, byte);
	SET_DFC(dfc);
}
