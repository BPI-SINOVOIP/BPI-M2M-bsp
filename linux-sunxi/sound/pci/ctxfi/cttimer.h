/*
 * sound/pci/ctxfi/cttimer.h
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
 * Timer handling
 */

#ifndef __CTTIMER_H
#define __CTTIMER_H

#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/list.h>

struct snd_pcm_substream;
struct ct_atc;
struct ct_atc_pcm;

struct ct_timer;
struct ct_timer_instance;

struct ct_timer *ct_timer_new(struct ct_atc *atc);
void ct_timer_free(struct ct_timer *atimer);

struct ct_timer_instance *
ct_timer_instance_new(struct ct_timer *atimer, struct ct_atc_pcm *apcm);
void ct_timer_instance_free(struct ct_timer_instance *ti);
void ct_timer_start(struct ct_timer_instance *ti);
void ct_timer_stop(struct ct_timer_instance *ti);
void ct_timer_prepare(struct ct_timer_instance *ti);

#endif /* __CTTIMER_H */
