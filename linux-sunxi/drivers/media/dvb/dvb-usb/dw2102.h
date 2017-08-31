/*
 * drivers/media/dvb/dvb-usb/dw2102.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _DW2102_H_
#define _DW2102_H_

#define DVB_USB_LOG_PREFIX "dw2102"
#include "dvb-usb.h"

#define deb_xfer(args...) dprintk(dvb_usb_dw2102_debug, 0x02, args)
#define deb_rc(args...)   dprintk(dvb_usb_dw2102_debug, 0x04, args)
#endif
