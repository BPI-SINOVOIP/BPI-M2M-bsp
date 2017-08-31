/*
 * include/linux/usb/isp116x.h
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
 * Board initialization code should put one of these into dev->platform_data
 * and place the isp116x onto platform_bus.
 */

#ifndef __LINUX_USB_ISP116X_H
#define __LINUX_USB_ISP116X_H

struct isp116x_platform_data {
	/* Enable internal resistors on downstream ports */
	unsigned sel15Kres:1;
	/* On-chip overcurrent detection */
	unsigned oc_enable:1;
	/* INT output polarity */
	unsigned int_act_high:1;
	/* INT edge or level triggered */
	unsigned int_edge_triggered:1;
	/* Enable wakeup by devices on usb bus (e.g. wakeup
	   by attachment/detachment or by device activity
	   such as moving a mouse). When chosen, this option
	   prevents stopping internal clock, increasing
	   thereby power consumption in suspended state. */
	unsigned remote_wakeup_enable:1;
	/* Inter-io delay (ns). The chip is picky about access timings; it
	   expects at least:
	   150ns delay between consecutive accesses to DATA_REG,
	   300ns delay between access to ADDR_REG and DATA_REG
	   OE, WE MUST NOT be changed during these intervals
	 */
	void (*delay) (struct device *dev, int delay);
};

#endif /* __LINUX_USB_ISP116X_H */
