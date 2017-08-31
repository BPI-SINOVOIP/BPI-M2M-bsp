/*
 * arch/cris/include/asm/rs485.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* RS-485 structures */

/* Used with ioctl() TIOCSERSETRS485 for backward compatibility!
 * XXX: Do not use it for new code!
 */
struct rs485_control {
        unsigned short rts_on_send;
        unsigned short rts_after_sent;
        unsigned long delay_rts_before_send;
        unsigned short enabled;
};

/* Used with ioctl() TIOCSERWRRS485 */
struct rs485_write {
        unsigned short outc_size;
        unsigned char *outc;
};

