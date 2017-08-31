/*
 * sound/usb/caiaq/audio.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef CAIAQ_AUDIO_H
#define CAIAQ_AUDIO_H

int snd_usb_caiaq_audio_init(struct snd_usb_caiaqdev *dev);
void snd_usb_caiaq_audio_free(struct snd_usb_caiaqdev *dev);

#endif /* CAIAQ_AUDIO_H */
