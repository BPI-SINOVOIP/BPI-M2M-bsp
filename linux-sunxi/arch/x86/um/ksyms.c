/*
 * arch/x86/um/ksyms.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <asm/string.h>
#include <asm/checksum.h>

#ifndef CONFIG_X86_32
/*XXX: we need them because they would be exported by x86_64 */
#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 3) || __GNUC__ > 4
EXPORT_SYMBOL(memcpy);
#else
EXPORT_SYMBOL(__memcpy);
#endif
#endif
EXPORT_SYMBOL(csum_partial);
