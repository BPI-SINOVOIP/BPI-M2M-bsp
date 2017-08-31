/*
 * drivers/staging/bcm/InterfaceMacros.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _INTERFACE_MACROS_H
#define _INTERFACE_MACROS_H

#define BCM_USB_MAX_READ_LENGTH 2048

#define MAXIMUM_USB_TCB      128
#define MAXIMUM_USB_RCB 	 128

#define MAX_BUFFERS_PER_QUEUE   256

#define MAX_DATA_BUFFER_SIZE    2048

//Num of Asynchronous reads pending
#define NUM_RX_DESC 64

#define SYS_CFG 0x0F000C00

#endif
