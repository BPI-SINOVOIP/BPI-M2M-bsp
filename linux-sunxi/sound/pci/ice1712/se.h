/*
 * sound/pci/ice1712/se.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SOUND_SE_H
#define __SOUND_SE_H

/* ID */
#define SE_DEVICE_DESC	\
		"{ONKYO INC,SE-90PCI},"\
		"{ONKYO INC,SE-200PCI},"

#define VT1724_SUBDEVICE_SE90PCI	0xb161000
#define VT1724_SUBDEVICE_SE200PCI	0xb160100

/* entry struct */
extern struct snd_ice1712_card_info snd_vt1724_se_cards[];

#endif /* __SOUND_SE_H */
