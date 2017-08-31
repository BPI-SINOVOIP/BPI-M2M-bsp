/*
 * sound/usb/mixer_quirks.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef SND_USB_MIXER_QUIRKS_H
#define SND_USB_MIXER_QUIRKS_H

int snd_usb_mixer_apply_create_quirk(struct usb_mixer_interface *mixer);

void snd_emuusb_set_samplerate(struct snd_usb_audio *chip,
			       unsigned char samplerate_id);

void snd_usb_mixer_rc_memory_change(struct usb_mixer_interface *mixer,
				    int unitid);

#endif /* SND_USB_MIXER_QUIRKS_H */

