/*
 * arch/x86/um/asm/segment.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __UM_SEGMENT_H
#define __UM_SEGMENT_H

extern int host_gdt_entry_tls_min;

#define GDT_ENTRY_TLS_ENTRIES 3
#define GDT_ENTRY_TLS_MIN host_gdt_entry_tls_min
#define GDT_ENTRY_TLS_MAX (GDT_ENTRY_TLS_MIN + GDT_ENTRY_TLS_ENTRIES - 1)

#endif
