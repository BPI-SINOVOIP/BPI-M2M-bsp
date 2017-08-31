/*
 * arch/arm/kernel/jump_label.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/jump_label.h>

#include "insn.h"
#include "patch.h"

#ifdef HAVE_JUMP_LABEL

static void __arch_jump_label_transform(struct jump_entry *entry,
					enum jump_label_type type,
					bool is_static)
{
	void *addr = (void *)entry->code;
	unsigned int insn;

	if (type == JUMP_LABEL_ENABLE)
		insn = arm_gen_branch(entry->code, entry->target);
	else
		insn = arm_gen_nop();

	if (is_static)
		__patch_text(addr, insn);
	else
		patch_text(addr, insn);
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	__arch_jump_label_transform(entry, type, false);
}

void arch_jump_label_transform_static(struct jump_entry *entry,
				      enum jump_label_type type)
{
	__arch_jump_label_transform(entry, type, true);
}

#endif
