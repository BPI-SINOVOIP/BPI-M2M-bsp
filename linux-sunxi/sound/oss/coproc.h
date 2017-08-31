/*
 * sound/oss/coproc.h
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
 * Definitions for various on board processors on the sound cards. For
 * example DSP processors.
 */

/*
 * Coprocessor access types
 */
#define COPR_CUSTOM		0x0001	/* Custom applications */
#define COPR_MIDI		0x0002	/* MIDI (MPU-401) emulation */
#define COPR_PCM		0x0004	/* Digitized voice applications */
#define COPR_SYNTH		0x0008	/* Music synthesis */
