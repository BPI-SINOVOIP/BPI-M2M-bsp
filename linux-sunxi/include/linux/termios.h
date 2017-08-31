/*
 * include/linux/termios.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _LINUX_TERMIOS_H
#define _LINUX_TERMIOS_H

#include <linux/types.h>
#include <asm/termios.h>

#define NFF	5

struct termiox
{
	__u16	x_hflag;
	__u16	x_cflag;
	__u16	x_rflag[NFF];
	__u16	x_sflag;
};

#define	RTSXOFF		0x0001		/* RTS flow control on input */
#define	CTSXON		0x0002		/* CTS flow control on output */
#define	DTRXOFF		0x0004		/* DTR flow control on input */
#define DSRXON		0x0008		/* DCD flow control on output */

#endif
