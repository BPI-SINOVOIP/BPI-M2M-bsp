/*
 * arch/ia64/include/asm/machvec_xen.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ASM_IA64_MACHVEC_XEN_h
#define _ASM_IA64_MACHVEC_XEN_h

extern ia64_mv_setup_t			dig_setup;
extern ia64_mv_cpu_init_t		xen_cpu_init;
extern ia64_mv_irq_init_t		xen_irq_init;
extern ia64_mv_send_ipi_t		xen_platform_send_ipi;

/*
 * This stuff has dual use!
 *
 * For a generic kernel, the macros are used to initialize the
 * platform's machvec structure.  When compiling a non-generic kernel,
 * the macros are used directly.
 */
#define platform_name				"xen"
#define platform_setup				dig_setup
#define platform_cpu_init			xen_cpu_init
#define platform_irq_init			xen_irq_init
#define platform_send_ipi			xen_platform_send_ipi

#endif /* _ASM_IA64_MACHVEC_XEN_h */
