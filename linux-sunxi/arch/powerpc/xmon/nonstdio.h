/*
 * arch/powerpc/xmon/nonstdio.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#define EOF	(-1)

#define printf	xmon_printf
#define putchar	xmon_putchar

extern int xmon_putchar(int c);
extern int xmon_getchar(void);
extern void xmon_puts(const char *);
extern char *xmon_gets(char *, int);
extern void xmon_printf(const char *, ...);
extern void xmon_map_scc(void);
extern int xmon_expect(const char *str, unsigned long timeout);
extern int xmon_write(const void *ptr, int nb);
extern int xmon_readchar(void);
extern int xmon_read_poll(void);
