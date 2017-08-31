/*
 * include/linux/kbd_diacr.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _DIACR_H
#define _DIACR_H
#include <linux/kd.h>

extern struct kbdiacruc accent_table[];
extern unsigned int accent_table_size;

#endif /* _DIACR_H */
