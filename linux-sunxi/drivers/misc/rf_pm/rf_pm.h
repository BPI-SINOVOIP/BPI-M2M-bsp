/*
 * drivers/misc/rf_pm/rf_pm.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef RF__PM__H
#define RF__PM__H

//rf module common info
struct rf_mod_info {
  int       num;
  char      *power[4];
  int       power_vol[4];
  int       power_switch;
  int       chip_en;
  char      *lpo_use_apclk; 
};

//wl function info
struct wl_func_info {
	int  wifi_used;
  char *module_name;
  int  wl_reg_on;
  int  wl_power_state;

#ifdef CONFIG_PROC_FS
	struct proc_dir_entry		*proc_root;
	struct proc_dir_entry		*proc_power;
#endif  
};

extern int sunxi_gpio_req(struct gpio_config *gpio);
#endif

