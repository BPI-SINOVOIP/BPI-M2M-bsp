/*
 * include/linux/mmc/sdhci-pci-data.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef LINUX_MMC_SDHCI_PCI_DATA_H
#define LINUX_MMC_SDHCI_PCI_DATA_H

struct pci_dev;

struct sdhci_pci_data {
	struct pci_dev	*pdev;
	int		slotno;
	int		rst_n_gpio; /* Set to -EINVAL if unused */
	int		cd_gpio;    /* Set to -EINVAL if unused */
	int		(*setup)(struct sdhci_pci_data *data);
	void		(*cleanup)(struct sdhci_pci_data *data);
};

extern struct sdhci_pci_data *(*sdhci_pci_get_data)(struct pci_dev *pdev,
				int slotno);

#endif
