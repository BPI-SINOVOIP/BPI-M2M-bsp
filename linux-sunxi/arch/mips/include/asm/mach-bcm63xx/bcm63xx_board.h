/*
 * arch/mips/include/asm/mach-bcm63xx/bcm63xx_board.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef BCM63XX_BOARD_H_
#define BCM63XX_BOARD_H_

const char *board_get_name(void);

void board_prom_init(void);

void board_setup(void);

int board_register_devices(void);

#endif /* ! BCM63XX_BOARD_H_ */
