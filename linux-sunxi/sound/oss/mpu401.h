/*
 * sound/oss/mpu401.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

/*	From uart401.c */
int probe_uart401 (struct address_info *hw_config, struct module *owner);
void unload_uart401 (struct address_info *hw_config);

irqreturn_t uart401intr (int irq, void *dev_id);

/*	From mpu401.c */
int probe_mpu401(struct address_info *hw_config, struct resource *ports);
int attach_mpu401(struct address_info * hw_config, struct module *owner);
void unload_mpu401(struct address_info *hw_info);
