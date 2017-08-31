/*
 * sound/oss/sb_ess.h
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
 * Created: 9-Jan-1999 Rolf Fokkens
 */

extern void ess_intr
		(sb_devc *devc);
extern int ess_dsp_init
		(sb_devc *devc, struct address_info *hw_config);

extern struct audio_driver *ess_audio_init
		(sb_devc *devc, int *audio_flags, int *format_mask);
extern int ess_midi_init
		(sb_devc *devc, struct address_info *hw_config);
extern void ess_mixer_init
		(sb_devc *devc);

extern int ess_init
		(sb_devc *devc, struct address_info *hw_config);
extern int ess_dsp_reset
		(sb_devc *devc);

extern void ess_setmixer
		(sb_devc *devc, unsigned int port, unsigned int value);
extern unsigned int ess_getmixer
		(sb_devc *devc, unsigned int port);
extern int ess_mixer_set
		(sb_devc *devc, int dev, int left, int right);
extern int ess_mixer_reset
		(sb_devc *devc);
extern void ess_mixer_reload
		(sb_devc * devc, int dev);
extern int ess_set_recmask
		(sb_devc *devc, int *mask);

