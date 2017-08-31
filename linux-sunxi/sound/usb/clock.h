/*
 * sound/usb/clock.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __USBAUDIO_CLOCK_H
#define __USBAUDIO_CLOCK_H

int snd_usb_init_sample_rate(struct snd_usb_audio *chip, int iface,
			     struct usb_host_interface *alts,
			     struct audioformat *fmt, int rate);

int snd_usb_clock_find_source(struct snd_usb_audio *chip, int entity_id);

#endif /* __USBAUDIO_CLOCK_H */
