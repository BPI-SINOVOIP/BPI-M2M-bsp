/*
 * drivers/media/dvb/dvb-usb/gl861.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _DVB_USB_GL861_H_
#define _DVB_USB_GL861_H_

#define DVB_USB_LOG_PREFIX "gl861"
#include "dvb-usb.h"

#define deb_rc(args...)   dprintk(dvb_usb_gl861_debug, 0x01, args)

#define GL861_WRITE		0x40
#define GL861_READ		0xc0

#define GL861_REQ_I2C_WRITE	0x01
#define GL861_REQ_I2C_READ	0x02

#endif
