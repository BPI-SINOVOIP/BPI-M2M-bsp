/*
 * lib/xz/xz_dec_syms.c
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
 * XZ decoder module information
 *
 * Author: Lasse Collin <lasse.collin@tukaani.org>
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

#include <linux/module.h>
#include <linux/xz.h>

EXPORT_SYMBOL(xz_dec_init);
EXPORT_SYMBOL(xz_dec_reset);
EXPORT_SYMBOL(xz_dec_run);
EXPORT_SYMBOL(xz_dec_end);

MODULE_DESCRIPTION("XZ decompressor");
MODULE_VERSION("1.0");
MODULE_AUTHOR("Lasse Collin <lasse.collin@tukaani.org> and Igor Pavlov");

/*
 * This code is in the public domain, but in Linux it's simplest to just
 * say it's GPL and consider the authors as the copyright holders.
 */
MODULE_LICENSE("GPL");
