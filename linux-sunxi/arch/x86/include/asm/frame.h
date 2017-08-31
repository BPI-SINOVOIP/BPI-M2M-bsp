/*
 * arch/x86/include/asm/frame.h
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
#include <asm/dwarf2.h>

/* The annotation hides the frame from the unwinder and makes it look
   like a ordinary ebp save/restore. This avoids some special cases for
   frame pointer later */
#ifdef CONFIG_FRAME_POINTER
	.macro FRAME
	__ASM_SIZE(push,_cfi)	%__ASM_REG(bp)
	CFI_REL_OFFSET		__ASM_REG(bp), 0
	__ASM_SIZE(mov)		%__ASM_REG(sp), %__ASM_REG(bp)
	.endm
	.macro ENDFRAME
	__ASM_SIZE(pop,_cfi)	%__ASM_REG(bp)
	CFI_RESTORE		__ASM_REG(bp)
	.endm
#else
	.macro FRAME
	.endm
	.macro ENDFRAME
	.endm
#endif

#endif  /*  __ASSEMBLY__  */
