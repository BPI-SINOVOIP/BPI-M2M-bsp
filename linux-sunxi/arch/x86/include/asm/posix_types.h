/*
 * arch/x86/include/asm/posix_types.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef __KERNEL__
# ifdef CONFIG_X86_32
#  include "posix_types_32.h"
# else
#  include "posix_types_64.h"
# endif
#else
# ifdef __i386__
#  include "posix_types_32.h"
# elif defined(__ILP32__)
#  include "posix_types_x32.h"
# else
#  include "posix_types_64.h"
# endif
#endif
