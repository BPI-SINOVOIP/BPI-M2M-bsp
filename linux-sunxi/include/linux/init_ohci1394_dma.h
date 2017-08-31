/*
 * include/linux/init_ohci1394_dma.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_PROVIDE_OHCI1394_DMA_INIT
extern int __initdata init_ohci1394_dma_early;
extern void __init init_ohci1394_dma_on_all_controllers(void);
#endif
