/*
 * drivers/staging/keucr/scsiglue.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SCSIGLUE_H_
#define _SCSIGLUE_H_

extern void usb_stor_report_device_reset(struct us_data *us);
extern void usb_stor_report_bus_reset(struct us_data *us);

extern unsigned char usb_stor_sense_invalidCDB[18];
extern struct scsi_host_template usb_stor_host_template;

#endif
