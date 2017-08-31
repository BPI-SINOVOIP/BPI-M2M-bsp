/*
 * arch/sh/include/cpu-sh3/cpu/serial.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __CPU_SH3_SERIAL_H
#define __CPU_SH3_SERIAL_H

#include <linux/serial_sci.h>

extern struct plat_sci_port_ops sh770x_sci_port_ops;
extern struct plat_sci_port_ops sh7710_sci_port_ops;
extern struct plat_sci_port_ops sh7720_sci_port_ops;

#endif /* __CPU_SH3_SERIAL_H */
