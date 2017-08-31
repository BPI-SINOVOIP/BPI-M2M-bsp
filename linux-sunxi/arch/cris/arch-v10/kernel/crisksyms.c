/*
 * arch/cris/arch-v10/kernel/crisksyms.c
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
#include <asm/io.h>
#include <arch/svinto.h>

/* Export shadow registers for the CPU I/O pins */
EXPORT_SYMBOL(genconfig_shadow);
EXPORT_SYMBOL(port_pa_data_shadow);
EXPORT_SYMBOL(port_pa_dir_shadow);
EXPORT_SYMBOL(port_pb_data_shadow);
EXPORT_SYMBOL(port_pb_dir_shadow);
EXPORT_SYMBOL(port_pb_config_shadow);
EXPORT_SYMBOL(port_g_data_shadow);

/* Cache flush functions */
EXPORT_SYMBOL(flush_etrax_cache);
EXPORT_SYMBOL(prepare_rx_descriptor);
