/*
 * sound/pci/au88x0/au88x0_mixer.c
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
 * Vortex Mixer support.
 *
 * There is much more than just the AC97 mixer...
 *
 */

#include <linux/time.h>
#include <linux/init.h>
#include <sound/core.h>
#include "au88x0.h"

static int __devinit snd_vortex_mixer(vortex_t * vortex)
{
	struct snd_ac97_bus *pbus;
	struct snd_ac97_template ac97;
	int err;
	static struct snd_ac97_bus_ops ops = {
		.write = vortex_codec_write,
		.read = vortex_codec_read,
	};

	if ((err = snd_ac97_bus(vortex->card, 0, &ops, NULL, &pbus)) < 0)
		return err;
	memset(&ac97, 0, sizeof(ac97));
	// Initialize AC97 codec stuff.
	ac97.private_data = vortex;
	ac97.scaps = AC97_SCAP_NO_SPDIF;
	err = snd_ac97_mixer(pbus, &ac97, &vortex->codec);
	vortex->isquad = ((vortex->codec == NULL) ?  0 : (vortex->codec->ext_id&0x80));
	return err;
}
