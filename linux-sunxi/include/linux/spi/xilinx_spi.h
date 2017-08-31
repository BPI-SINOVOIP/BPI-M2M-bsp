/*
 * include/linux/spi/xilinx_spi.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_SPI_XILINX_SPI_H
#define __LINUX_SPI_XILINX_SPI_H

/**
 * struct xspi_platform_data - Platform data of the Xilinx SPI driver
 * @num_chipselect:	Number of chip select by the IP.
 * @little_endian:	If registers should be accessed little endian or not.
 * @bits_per_word:	Number of bits per word.
 * @devices:		Devices to add when the driver is probed.
 * @num_devices:	Number of devices in the devices array.
 */
struct xspi_platform_data {
	u16 num_chipselect;
	bool little_endian;
	u8 bits_per_word;
	struct spi_board_info *devices;
	u8 num_devices;
};

#endif /* __LINUX_SPI_XILINX_SPI_H */
