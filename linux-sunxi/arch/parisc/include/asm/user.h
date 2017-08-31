/*
 * arch/parisc/include/asm/user.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* This file should not exist, but lots of generic code still includes
   it. It's a hangover from old a.out days and the traditional core
   dump format.  We are ELF-only, and so are our core dumps.  If we
   need to support HP/UX core format then we'll do it here
   eventually. */
