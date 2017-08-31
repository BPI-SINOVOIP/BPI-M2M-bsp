/*
 * net/compat-wireless/include/linux/bcma/bcma_soc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_BCMA_SOC_H_
#define LINUX_BCMA_SOC_H_

#include <linux/bcma/bcma.h>

struct bcma_soc {
	struct bcma_bus bus;
	struct bcma_device core_cc;
	struct bcma_device core_mips;
};

int __init bcma_host_soc_register(struct bcma_soc *soc);

int bcma_bus_register(struct bcma_bus *bus);

#endif /* LINUX_BCMA_SOC_H_ */
