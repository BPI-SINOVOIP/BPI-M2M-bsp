/*
 * arch/c6x/include/asm/linkage.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_C6X_LINKAGE_H
#define _ASM_C6X_LINKAGE_H

#ifdef __ASSEMBLER__

#define __ALIGN		.align 2
#define __ALIGN_STR	".align 2"

#ifndef __DSBT__
#define ENTRY(name)		\
	.global name @		\
	__ALIGN @		\
name:
#else
#define ENTRY(name)		\
	.global name @		\
	.hidden name @		\
	__ALIGN @		\
name:
#endif

#define ENDPROC(name)		\
	.type name, @function @	\
	.size name, . - name

#endif

#include <asm-generic/linkage.h>

#endif /* _ASM_C6X_LINKAGE_H */
