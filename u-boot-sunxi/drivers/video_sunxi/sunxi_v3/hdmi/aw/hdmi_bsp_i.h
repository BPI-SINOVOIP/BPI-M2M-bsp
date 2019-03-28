/*
 * Allwinner SoCs hdmi driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __HDMI_BSP_I_H_
#define __HDMI_BSP_I_H_

#define get_bvalue(addr)	(*((volatile unsigned char  *)(addr)))
#define put_bvalue(addr, v)	(*((volatile unsigned char  *)(addr)) = (unsigned char)(v))
#define get_hvalue(addr)		(*((volatile unsigned short *)(addr)))
#define put_hvalue(addr, v)	(*((volatile unsigned short *)(addr)) = (unsigned short)(v))
#define get_wvalue(addr)		(*((volatile unsigned long  *)(addr)))
#define put_wvalue(addr, v)	(*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

/* #define set_bit(addr, v)    (*((volatile unsigned char  *)(addr)) |=  (unsigned char)(v)) */
/* #define clr_bit(addr, v)    (*((volatile unsigned char  *)(addr)) &= ~(unsigned char)(v)) */
#define set_bbit(addr, v)   (*((volatile unsigned char  *)(addr)) |=  (unsigned char)(v))
#define clr_bbit(addr, v)   (*((volatile unsigned char  *)(addr)) &= ~(unsigned char)(v))
#define set_hbit(addr, v)   (*((volatile unsigned short *)(addr)) |=  (unsigned short)(v))
#define clr_hbit(addr, v)   (*((volatile unsigned short *)(addr)) &= ~(unsigned short)(v))
#define set_wbit(addr, v)   (*((volatile unsigned long  *)(addr)) |=  (unsigned long)(v))
#define clr_wbit(addr, v)   (*((volatile unsigned long  *)(addr)) &= ~(unsigned long)(v))

#endif
