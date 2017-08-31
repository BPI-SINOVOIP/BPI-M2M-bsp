/*
 * arch/arm/mach-ep93xx/include/mach/ep93xx_spi.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __ASM_MACH_EP93XX_SPI_H
#define __ASM_MACH_EP93XX_SPI_H

struct spi_device;

/**
 * struct ep93xx_spi_info - EP93xx specific SPI descriptor
 * @num_chipselect: number of chip selects on this board, must be
 *                  at least one
 * @use_dma: use DMA for the transfers
 */
struct ep93xx_spi_info {
	int	num_chipselect;
	bool	use_dma;
};

/**
 * struct ep93xx_spi_chip_ops - operation callbacks for SPI slave device
 * @setup: setup the chip select mechanism
 * @cleanup: cleanup the chip select mechanism
 * @cs_control: control the device chip select
 */
struct ep93xx_spi_chip_ops {
	int	(*setup)(struct spi_device *spi);
	void	(*cleanup)(struct spi_device *spi);
	void	(*cs_control)(struct spi_device *spi, int value);
};

#endif /* __ASM_MACH_EP93XX_SPI_H */
