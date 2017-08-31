/*
 * arch/m68k/include/asm/atari_stdma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef _atari_stdma_h
#define _atari_stdma_h


#include <linux/interrupt.h>


/***************************** Prototypes *****************************/

void stdma_lock(irq_handler_t handler, void *data);
void stdma_release( void );
int stdma_others_waiting( void );
int stdma_islocked( void );
void *stdma_locked_by( void );
void stdma_init( void );

/************************* End of Prototypes **************************/



#endif  /* _atari_stdma_h */
