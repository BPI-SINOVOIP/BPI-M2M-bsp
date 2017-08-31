/*
 * drivers/parport/multiface.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _MULTIFACE_H_
#define _MULTIFACE_H_

/*
 * Defines for SerialMaster, Multiface Card II and Multiface Card III
 * The addresses given below are offsets to the board base address
 * 
 * 6.11.95 Joerg Dorchain (dorchain@mpi-sb.mpg.de)
 *
 */

#define PIA_REG_PADWIDTH 255

#define DUARTBASE 0x0000
#define PITBASE   0x0100
#define ROMBASE   0x0200
#define PIABASE   0x4000

#endif

