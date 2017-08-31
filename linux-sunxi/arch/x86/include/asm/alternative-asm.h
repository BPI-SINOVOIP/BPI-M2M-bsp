/*
 * arch/x86/include/asm/alternative-asm.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef __ASSEMBLY__

#include <asm/asm.h>

#ifdef CONFIG_SMP
	.macro LOCK_PREFIX
672:	lock
	.section .smp_locks,"a"
	.balign 4
	.long 672b - .
	.previous
	.endm
#else
	.macro LOCK_PREFIX
	.endm
#endif

.macro altinstruction_entry orig alt feature orig_len alt_len
	.long \orig - .
	.long \alt - .
	.word \feature
	.byte \orig_len
	.byte \alt_len
.endm

#endif  /*  __ASSEMBLY__  */
