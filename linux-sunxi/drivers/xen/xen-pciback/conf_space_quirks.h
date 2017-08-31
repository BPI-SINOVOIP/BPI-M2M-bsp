/*
 * drivers/xen/xen-pciback/conf_space_quirks.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * PCI Backend - Data structures for special overlays for broken devices.
 *
 * Ryan Wilson <hap9@epoch.ncsc.mil>
 * Chris Bookholt <hap10@epoch.ncsc.mil>
 */

#ifndef __XEN_PCIBACK_CONF_SPACE_QUIRKS_H__
#define __XEN_PCIBACK_CONF_SPACE_QUIRKS_H__

#include <linux/pci.h>
#include <linux/list.h>

struct xen_pcibk_config_quirk {
	struct list_head quirks_list;
	struct pci_device_id devid;
	struct pci_dev *pdev;
};

int xen_pcibk_config_quirks_add_field(struct pci_dev *dev, struct config_field
				    *field);

int xen_pcibk_config_quirks_remove_field(struct pci_dev *dev, int reg);

int xen_pcibk_config_quirks_init(struct pci_dev *dev);

void xen_pcibk_config_field_free(struct config_field *field);

int xen_pcibk_config_quirk_release(struct pci_dev *dev);

int xen_pcibk_field_is_dup(struct pci_dev *dev, unsigned int reg);

#endif
