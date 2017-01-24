/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __DEV_LCD_H__
#define __DEV_LCD_H__

//#include "panels/panels.h"
#include "lcd_panel_cfg.h"
//#include "../../dev_disp.h"
extern int sunxi_disp_get_source_ops(struct sunxi_disp_source_ops *src_ops);

extern struct sunxi_lcd_drv g_lcd_drv;

#if 0
#define OSAL_PRINTF(msg...) {printk(KERN_WARNING "[LCD] ");printk(msg);}
#define __inf(msg...)       {printk(KERN_WARNING "[LCD] ");printk(msg);}
#define __msg(msg...)       {printk(KERN_WARNING "[LCD] file:%s,line:%d:    ",__FILE__,__LINE__);printk(msg);}
#define __wrn(msg...)       {printk(KERN_WARNING "[LCD WRN] file:%s,line:%d:    ",__FILE__,__LINE__); printk(msg);}
#define __here__            {printk(KERN_WARNING "[LCD] file:%s,line:%d\n",__FILE__,__LINE__);}
#endif

#if 0
#define OSAL_PRINTF
#define __inf(msg...)
#define __msg(msg...)
#define __wrn(msg...)
#define __here__
#endif

struct sunxi_lcd_drv
{
  struct sunxi_disp_source_ops      src_ops;
};

int lcd_init(void);

#endif
