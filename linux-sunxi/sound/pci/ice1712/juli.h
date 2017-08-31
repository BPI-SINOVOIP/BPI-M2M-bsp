/*
 * sound/pci/ice1712/juli.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __SOUND_JULI_H
#define __SOUND_JULI_H

#define JULI_DEVICE_DESC		"{ESI,Juli@},"

#define VT1724_SUBDEVICE_JULI		0x31305345	/* Juli@ */

extern struct snd_ice1712_card_info  snd_vt1724_juli_cards[];

#endif	/* __SOUND_JULI_H */
