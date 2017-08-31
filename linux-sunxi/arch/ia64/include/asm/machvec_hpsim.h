/*
 * arch/ia64/include/asm/machvec_hpsim.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_MACHVEC_HPSIM_h
#define _ASM_IA64_MACHVEC_HPSIM_h

extern ia64_mv_setup_t hpsim_setup;
extern ia64_mv_irq_init_t hpsim_irq_init;

/*
 * This stuff has dual use!
 *
 * For a generic kernel, the macros are used to initialize the
 * platform's machvec structure.  When compiling a non-generic kernel,
 * the macros are used directly.
 */
#define platform_name		"hpsim"
#define platform_setup		hpsim_setup
#define platform_irq_init	hpsim_irq_init

#endif /* _ASM_IA64_MACHVEC_HPSIM_h */
