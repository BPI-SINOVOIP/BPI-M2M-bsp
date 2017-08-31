/*
 * include/asm-generic/serial.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_GENERIC_SERIAL_H
#define __ASM_GENERIC_SERIAL_H

/*
 * This should not be an architecture specific #define, oh well.
 *
 * Traditionally, it just describes i8250 and related serial ports
 * that have this clock rate.
 */

#define BASE_BAUD (1843200 / 16)

#endif /* __ASM_GENERIC_SERIAL_H */
