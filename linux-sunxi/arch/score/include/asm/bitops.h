/*
 * arch/score/include/asm/bitops.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_SCORE_BITOPS_H
#define _ASM_SCORE_BITOPS_H

#include <asm/byteorder.h> /* swab32 */

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()

#include <asm-generic/bitops.h>
#include <asm-generic/bitops/__fls.h>

#endif /* _ASM_SCORE_BITOPS_H */
