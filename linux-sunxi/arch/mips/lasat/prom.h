/*
 * arch/mips/lasat/prom.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __PROM_H
#define __PROM_H

extern void (*prom_display)(const char *string, int pos, int clear);
extern void (*prom_monitor)(void);

#endif /* __PROM_H */
