/*
 * include/linux/s3c_adc_battery.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _S3C_ADC_BATTERY_H
#define _S3C_ADC_BATTERY_H

struct s3c_adc_bat_thresh {
	int volt; /* mV */
	int cur; /* mA */
	int level; /* percent */
};

struct s3c_adc_bat_pdata {
	int (*init)(void);
	void (*exit)(void);
	void (*enable_charger)(void);
	void (*disable_charger)(void);

	int gpio_charge_finished;
	int gpio_inverted;

	const struct s3c_adc_bat_thresh *lut_noac;
	unsigned int lut_noac_cnt;
	const struct s3c_adc_bat_thresh *lut_acin;
	unsigned int lut_acin_cnt;

	const unsigned int volt_channel;
	const unsigned int current_channel;
	const unsigned int backup_volt_channel;

	const unsigned int volt_samples;
	const unsigned int current_samples;
	const unsigned int backup_volt_samples;

	const unsigned int volt_mult;
	const unsigned int current_mult;
	const unsigned int backup_volt_mult;
	const unsigned int internal_impedance;

	const unsigned int backup_volt_max;
	const unsigned int backup_volt_min;
};

#endif
