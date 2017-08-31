/*
 * arch/h8300/include/asm/sh_bios.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* eCos HAL interface header */

#ifndef SH_BIOS_H
#define SH_BIOS_H

#define HAL_IF_VECTOR_TABLE 0xfffe20
#define CALL_IF_SET_CONSOLE_COMM  13
#define QUERY_CURRENT -1
#define MANGLER       -3

/* Checking for GDB stub active */
/* suggestion Jonathan Larmour */
static int sh_bios_in_gdb_mode(void)
{
	static int gdb_active = -1;
	if (gdb_active == -1) {
		int (*set_console_comm)(int);
		set_console_comm = ((void **)HAL_IF_VECTOR_TABLE)[CALL_IF_SET_CONSOLE_COMM];
		gdb_active = (set_console_comm(QUERY_CURRENT) == MANGLER);
	}
	return gdb_active;
}

static void sh_bios_gdb_detach(void)
{

}

#endif
