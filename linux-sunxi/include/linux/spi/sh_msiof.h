/*
 * include/linux/spi/sh_msiof.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SPI_SH_MSIOF_H__
#define __SPI_SH_MSIOF_H__

struct sh_msiof_spi_info {
	int tx_fifo_override;
	int rx_fifo_override;
	u16 num_chipselect;
};

#endif /* __SPI_SH_MSIOF_H__ */
