/*
 * drivers/staging/rtl8192u/r819xU_firmware_img.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef IMG_H
#define IMG_H

#define MACPHY_Array_PGLength 30
#define PHY_REG_1T2RArrayLength 296
#define AGCTAB_ArrayLength 384
#define MACPHY_ArrayLength 18

#define RadioA_ArrayLength 246
#define RadioB_ArrayLength 78
#define RadioC_ArrayLength 1
#define RadioD_ArrayLength 1
#define PHY_REGArrayLength 1


extern u32 Rtl8192UsbPHY_REGArray[];
extern u32 Rtl8192UsbPHY_REG_1T2RArray[];
extern u32 Rtl8192UsbRadioA_Array[];
extern u32 Rtl8192UsbRadioB_Array[];
extern u32 Rtl8192UsbRadioC_Array[];
extern u32 Rtl8192UsbRadioD_Array[];
extern u32 Rtl8192UsbMACPHY_Array[];
extern u32 Rtl8192UsbMACPHY_Array_PG[];
extern u32 Rtl8192UsbAGCTAB_Array[];



#endif
