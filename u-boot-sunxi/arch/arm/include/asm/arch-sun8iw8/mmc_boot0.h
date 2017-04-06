/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _SUNXI_MMC_BOOT0_H
#define _SUNXI_MMC_BOOT0_H

void set_mmc_para(int smc_no,void *sdly_addr );
unsigned long mmc_bread(int dev_num, unsigned long start, unsigned blkcnt, void *dst);
int sunxi_mmc_init(int sdc_no, unsigned bus_width, const normal_gpio_cfg *gpio_info, int offset ,void *extra_data);
int sunxi_mmc_exit(int sdc_no, const normal_gpio_cfg *gpio_info, int offset);

#endif /* _SUNXI_MMC_BOOT0_H */
