/*
 * arch/xtensa/platforms/iss/io.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* This file isn't really needed right now. */

#if 0

#include <asm/io.h>
#include <platform/platform-iss/simcall.h>

extern int __simc ();


char iss_serial_getc()
{
  char c;
  __simc( SYS_read, 0, &c, 1 );
  return c;
}

void iss_serial_putc( char c )
{
  __simc( SYS_write, 1, &c, 1 );
}

void iss_serial_puts( char *s )
{
  if( s != 0 && *s != 0 )
    __simc( SYS_write, 1, s, strlen(s) );
}

/*#error Need I/O ports to specific hardware!*/

#endif

