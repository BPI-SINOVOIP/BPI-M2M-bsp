/*
 * drivers/staging/bcm/InterfaceMisc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __INTERFACE_MISC_H
#define __INTERFACE_MISC_H

INT
InterfaceRDM(PS_INTERFACE_ADAPTER psIntfAdapter,
			UINT addr,
			PVOID buff,
			INT len);

INT
InterfaceWRM(PS_INTERFACE_ADAPTER psIntfAdapter,
			UINT addr,
			PVOID buff,
			INT len);


int InterfaceFileDownload( PVOID psIntfAdapter,
                        struct file *flp,
                        unsigned int on_chip_loc);

int InterfaceFileReadbackFromChip( PVOID psIntfAdapter,
                        struct file *flp,
                        unsigned int on_chip_loc);


int BcmRDM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len);

int BcmWRM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len);

INT Bcm_clear_halt_of_endpoints(PMINI_ADAPTER Adapter);

VOID Bcm_kill_all_URBs(PS_INTERFACE_ADAPTER psIntfAdapter);

#define DISABLE_USB_ZERO_LEN_INT 0x0F011878

#endif // __INTERFACE_MISC_H
