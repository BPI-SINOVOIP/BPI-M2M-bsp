/*
 * arch/m68k/include/asm/movs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef  __MOVS_H__
#define __MOVS_H__

/*
** movs.h
**
** Inline assembly macros to generate movs & related instructions
*/

/* Set DFC register value */

#define SET_DFC(x) \
        __asm__ __volatile__ (" movec %0,%/dfc" : : "d" (x));

/* Get DFC register value */

#define GET_DFC(x) \
        __asm__ __volatile__ (" movec %/dfc, %0" : "=d" (x) : );

/* Set SFC register value */

#define SET_SFC(x) \
        __asm__ __volatile__ (" movec %0,%/sfc" : : "d" (x));

/* Get SFC register value */

#define GET_SFC(x) \
        __asm__ __volatile__ (" movec %/sfc, %0" : "=d" (x) : );

#define SET_VBR(x) \
        __asm__ __volatile__ (" movec %0,%/vbr" : : "r" (x));

#define GET_VBR(x) \
        __asm__ __volatile__ (" movec %/vbr, %0" : "=g" (x) : );

/* Set a byte using the "movs" instruction */

#define SET_CONTROL_BYTE(addr,value) \
        __asm__ __volatile__ (" movsb %0, %1@" : : "d" (value), "a" (addr));

/* Get a byte using the "movs" instruction */

#define GET_CONTROL_BYTE(addr,value) \
        __asm__ __volatile__ (" movsb %1@, %0" : "=d" (value) : "a" (addr));

/* Set a (long)word using the "movs" instruction */

#define SET_CONTROL_WORD(addr,value) \
        __asm__ __volatile__ (" movsl %0, %1@" : : "d" (value), "a" (addr));

/* Get a (long)word using the "movs" instruction */

#define GET_CONTROL_WORD(addr,value) \
        __asm__ __volatile__ (" movsl %1@, %0" : "=d" (value) : "a" (addr));
#endif
