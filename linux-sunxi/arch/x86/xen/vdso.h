/*
 * arch/x86/xen/vdso.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* Bit used for the pseudo-hwcap for non-negative segments.  We use
   bit 1 to avoid bugs in some versions of glibc when bit 0 is
   used; the choice is otherwise arbitrary. */
#define VDSO_NOTE_NONEGSEG_BIT	1
