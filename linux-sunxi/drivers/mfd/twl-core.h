/*
 * drivers/mfd/twl-core.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __TWL_CORE_H__
#define __TWL_CORE_H__

extern int twl6030_init_irq(struct device *dev, int irq_num);
extern int twl6030_exit_irq(void);
extern int twl4030_init_irq(struct device *dev, int irq_num);
extern int twl4030_exit_irq(void);
extern int twl4030_init_chip_irq(const char *chip);

#endif /*  __TWL_CORE_H__ */
