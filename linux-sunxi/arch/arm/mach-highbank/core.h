/*
 * arch/arm/mach-highbank/core.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
extern void highbank_set_cpu_jump(int cpu, void *jump_addr);
extern void highbank_clocks_init(void);
extern void highbank_restart(char, const char *);
extern void __iomem *scu_base_addr;
#ifdef CONFIG_DEBUG_HIGHBANK_UART
extern void highbank_lluart_map_io(void);
#else
static inline void highbank_lluart_map_io(void) {}
#endif

