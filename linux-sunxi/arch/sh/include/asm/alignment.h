/*
 * arch/sh/include/asm/alignment.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_SH_ALIGNMENT_H
#define __ASM_SH_ALIGNMENT_H

#include <linux/types.h>

extern void inc_unaligned_byte_access(void);
extern void inc_unaligned_word_access(void);
extern void inc_unaligned_dword_access(void);
extern void inc_unaligned_multi_access(void);
extern void inc_unaligned_user_access(void);
extern void inc_unaligned_kernel_access(void);

#define UM_WARN		(1 << 0)
#define UM_FIXUP	(1 << 1)
#define UM_SIGNAL	(1 << 2)

extern unsigned int unaligned_user_action(void);

extern void unaligned_fixups_notify(struct task_struct *, insn_size_t, struct pt_regs *);

#endif /* __ASM_SH_ALIGNMENT_H */
