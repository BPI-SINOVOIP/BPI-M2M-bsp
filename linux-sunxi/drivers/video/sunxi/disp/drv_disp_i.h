/*
 * drivers/video/sunxi/disp/drv_disp_i.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __DRV_DISP_I_H__
#define __DRV_DISP_I_H__

#include "de/bsp_display.h"
#include "de/disp_display.h"

#define HANDTOID(handle)  ((handle) - 100)
#define IDTOHAND(ID)  ((ID) + 100)


#define DISP_IO_NUM     8
#define DISP_IO_SCALER0 0
#define DISP_IO_SCALER1 1
#define DISP_IO_IMAGE0  2
#define DISP_IO_IMAGE1  3
#define DISP_IO_LCDC0   4
#define DISP_IO_LCDC1   5
#define DISP_IO_TVEC0    6
#define DISP_IO_TVEC1    7

#define sys_get_hvalue(n)   (*((volatile u16 *)(n)))         /* half word input */
#define sys_put_hvalue(n,c) (*((volatile u16 *)(n)) = (c))   /* half word output */
#define sys_get_wvalue(n)   (*((volatile u32 *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile u32 *)(n))  = (c))   /* word output */

#endif
