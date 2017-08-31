/*
 * drivers/misc/lradc_battery.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LRADC_BATTERY_H_
#define __LRADC_BATTERY_H_

#define LRADC_BASE_ADDR 0x01c22800

#define LRADC_CTRL              (0x00)
#define LRADC_INTC              (0x04)
#define LRADC_INT_STA           (0x08)
#define LRADC_DATA0             (0x0c)
#define LRADC_DATA1             (0x10)

#define FIRST_CONCERT_DLY       (0<<24)
#define CHAN                    (0x0)
#define ADC_CHAN_SELECT         (CHAN<<22)
#define LRADC_KEY_MODE          (0)
#define KEY_MODE_SELECT         (LRADC_KEY_MODE<<12)
#define LEVELB_VOL              (0<<4)
#define LRADC_HOLD_KEY_EN		(1<<7)
#define LRADC_HOLD_EN           (1<<6)
#define LRADC_SAMPLE_32HZ       (3<<2)
#define LRADC_SAMPLE_62HZ       (2<<2)
#define LRADC_SAMPLE_125HZ      (1<<2)
#define LRADC_SAMPLE_250HZ      (0<<2)
#define LRADC_EN                (1<<0)

#define LRADC_ADC1_UP_EN        (1<<12)
#define LRADC_ADC1_DOWN_EN      (1<<9)
#define LRADC_ADC1_DATA_EN      (1<<8)

#define LRADC_ADC0_UP_EN        (1<<4)
#define LRADC_ADC0_DOWN_EN      (1<<1)
#define LRADC_ADC0_DATA_EN      (1<<0)

#define LRADC_ADC1_UPPEND       (1<<12)
#define LRADC_ADC1_DOWNPEND     (1<<9)
#define LRADC_ADC1_DATAPEND     (1<<8)


#define LRADC_ADC0_UPPEND       (1<<4)
#define LRADC_ADC0_DOWNPEND     (1<<1)
#define LRADC_ADC0_DATAPEND     (1<<0)


/* interrutp control regitster */
#define LRADC_INT_DATA_IRQ_EN		(1<<0)
#define LRADC_INT_DOWN_IRQ_EN		(1<<1)
#define LRADC_INT_HOLD_IRQ_EN		(1<<2)
#define LRADC_INT_ALRDY_HOLD_IRQ_EN	(1<<3)
#define LRADC_INT_UP_IRQ_EN			(1<<4)


/* interrutp status regitster */
#define LRADC_INT_DATA_IRQ_ST		(1<<0)
#define LRADC_INT_DOWN_IRQ_ST		(1<<1)
#define LRADC_INT_HOLD_IRQ_ST		(1<<2)
#define LRADC_INT_ALRDY_HOLD_IRQ_ST	(1<<3)
#define LRADC_INT_UP_IRQ_ST			(1<<4)







#endif



