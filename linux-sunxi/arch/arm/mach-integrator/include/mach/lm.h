/*
 * arch/arm/mach-integrator/include/mach/lm.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

struct lm_device {
	struct device		dev;
	struct resource		resource;
	unsigned int		irq;
	unsigned int		id;
};

struct lm_driver {
	struct device_driver	drv;
	int			(*probe)(struct lm_device *);
	void			(*remove)(struct lm_device *);
	int			(*suspend)(struct lm_device *, pm_message_t);
	int			(*resume)(struct lm_device *);
};

int lm_driver_register(struct lm_driver *drv);
void lm_driver_unregister(struct lm_driver *drv);

int lm_device_register(struct lm_device *dev);

#define lm_get_drvdata(lm)	dev_get_drvdata(&(lm)->dev)
#define lm_set_drvdata(lm,d)	dev_set_drvdata(&(lm)->dev, d)
