/*
 * arch/cris/arch-v32/kernel/crisksyms.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/irq.h>
#include <arch/dma.h>
#include <arch/intmem.h>
#include <mach/pinmux.h>
#include <arch/io.h>

/* Functions for allocating DMA channels */
EXPORT_SYMBOL(crisv32_request_dma);
EXPORT_SYMBOL(crisv32_free_dma);

/* Functions for handling internal RAM */
EXPORT_SYMBOL(crisv32_intmem_alloc);
EXPORT_SYMBOL(crisv32_intmem_free);
EXPORT_SYMBOL(crisv32_intmem_phys_to_virt);
EXPORT_SYMBOL(crisv32_intmem_virt_to_phys);

/* Functions for handling pinmux */
EXPORT_SYMBOL(crisv32_pinmux_alloc);
EXPORT_SYMBOL(crisv32_pinmux_alloc_fixed);
EXPORT_SYMBOL(crisv32_pinmux_dealloc);
EXPORT_SYMBOL(crisv32_pinmux_dealloc_fixed);
EXPORT_SYMBOL(crisv32_io_get_name);
EXPORT_SYMBOL(crisv32_io_get);

/* Functions masking/unmasking interrupts */
EXPORT_SYMBOL(crisv32_mask_irq);
EXPORT_SYMBOL(crisv32_unmask_irq);
