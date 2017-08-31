/*
 * arch/mips/include/asm/txx9/boards.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_TOSHIBA_JMR3927
BOARD_VEC(jmr3927_vec)
#endif
#ifdef CONFIG_TOSHIBA_RBTX4927
BOARD_VEC(rbtx4927_vec)
BOARD_VEC(rbtx4937_vec)
#endif
#ifdef CONFIG_TOSHIBA_RBTX4938
BOARD_VEC(rbtx4938_vec)
#endif
#ifdef CONFIG_TOSHIBA_RBTX4939
BOARD_VEC(rbtx4939_vec)
#endif
