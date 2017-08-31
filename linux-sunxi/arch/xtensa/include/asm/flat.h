/*
 * arch/xtensa/include/asm/flat.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_XTENSA_FLAT_H
#define __ASM_XTENSA_FLAT_H

#define flat_argvp_envp_on_stack()			0
#define flat_old_ram_flag(flags)			(flags)
#define flat_reloc_valid(reloc, size)			((reloc) <= (size))
#define flat_get_addr_from_rp(rp, relval, flags, p)	get_unaligned(rp)
#define flat_put_addr_at_rp(rp, val, relval	)	put_unaligned(val, rp)
#define flat_get_relocate_addr(rel)			(rel)
#define flat_set_persistent(relval, p)			0

#endif /* __ASM_XTENSA_FLAT_H */
