/*
 * sound/usb/proc.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __USBAUDIO_PROC_H
#define __USBAUDIO_PROC_H

void snd_usb_audio_create_proc(struct snd_usb_audio *chip);
void snd_usb_proc_pcm_format_add(struct snd_usb_stream *stream);

#endif /* __USBAUDIO_PROC_H */

