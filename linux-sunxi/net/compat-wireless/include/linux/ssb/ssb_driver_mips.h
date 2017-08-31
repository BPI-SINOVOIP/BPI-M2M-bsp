/*
 * net/compat-wireless/include/linux/ssb/ssb_driver_mips.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_SSB_MIPSCORE_H_
#define LINUX_SSB_MIPSCORE_H_

#ifdef CONFIG_SSB_DRIVER_MIPS

struct ssb_device;

struct ssb_serial_port {
	void *regs;
	unsigned long clockspeed;
	unsigned int irq;
	unsigned int baud_base;
	unsigned int reg_shift;
};


struct ssb_mipscore {
	struct ssb_device *dev;

	int nr_serial_ports;
	struct ssb_serial_port serial_ports[4];

	u8 flash_buswidth;
	u32 flash_window;
	u32 flash_window_size;
};

extern void ssb_mipscore_init(struct ssb_mipscore *mcore);
extern u32 ssb_cpu_clock(struct ssb_mipscore *mcore);

extern unsigned int ssb_mips_irq(struct ssb_device *dev);


#else /* CONFIG_SSB_DRIVER_MIPS */

struct ssb_mipscore {
};

static inline
void ssb_mipscore_init(struct ssb_mipscore *mcore)
{
}

#endif /* CONFIG_SSB_DRIVER_MIPS */

#endif /* LINUX_SSB_MIPSCORE_H_ */
