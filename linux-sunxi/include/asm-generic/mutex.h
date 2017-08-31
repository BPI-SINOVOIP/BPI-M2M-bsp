/*
 * include/asm-generic/mutex.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_MUTEX_H
#define __ASM_GENERIC_MUTEX_H
/*
 * Pull in the generic implementation for the mutex fastpath,
 * which is a reasonable default on many architectures.
 */

#include <asm-generic/mutex-dec.h>
#endif /* __ASM_GENERIC_MUTEX_H */
