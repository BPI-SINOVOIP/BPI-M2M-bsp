/*
 * include/linux/usb/atmel_usba_udc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Platform data definitions for Atmel USBA gadget driver.
 */
#ifndef __LINUX_USB_USBA_H
#define __LINUX_USB_USBA_H

struct usba_ep_data {
	char	*name;
	int	index;
	int	fifo_size;
	int	nr_banks;
	int	can_dma;
	int	can_isoc;
};

struct usba_platform_data {
	int			vbus_pin;
	int			vbus_pin_inverted;
	int			num_ep;
	struct usba_ep_data	ep[0];
};

#endif /* __LINUX_USB_USBA_H */
