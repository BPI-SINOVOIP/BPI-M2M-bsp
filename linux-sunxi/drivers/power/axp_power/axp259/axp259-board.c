/*
 * drivers/power/axp_power/axp259/axp259-board.c
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/regulator/machine.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/delay.h>
#include <linux/power_supply.h>
#include <linux/apm_bios.h>
#include <linux/apm-emulation.h>
#include <linux/mfd/axp-mfd.h>
#include "axp259-mfd.h"
#include "../axp-cfg.h"

static struct axp_dev axp259_dev;

static struct power_supply_info battery_data = {
	.name = "PTI PL336078",
	.technology = POWER_SUPPLY_TECHNOLOGY_LiFe,
	.voltage_max_design = 4200000,
	.voltage_min_design = 3500000,
/*	.energy_full_design = pmu_battery_cap,*/
	.use_for_apm = 1,
};


static struct axp_supply_init_data axp_sply_init_data = {
	.battery_info = &battery_data,
	.chgcur = 1500000,
	.chgvol = 4200000,
	.chgend = 10,
	.chgen = 1,
	.sample_time = 800,
	.chgpretime = 50,
	.chgcsttime = 720,
};

static struct axp_funcdev_info axp_splydev[] = {
	{
		.name = "axp259-supplyer",
		.id = AXP22_ID_SUPPLY,
		.platform_data = &axp_sply_init_data,
	},
};

static struct axp_platform_data axp_pdata = {
	.num_sply_devs = ARRAY_SIZE(axp_splydev),
	.sply_devs = axp_splydev,
};

static struct axp_mfd_chip_ops axp259_ops[] = {
	[0] = {
		.init_chip    = axp259_init_chip,
		.enable_irqs  = axp259_enable_irqs,
		.disable_irqs = axp259_disable_irqs,
		.read_irqs    = axp259_read_irqs,
	},
};

void axp259_power_off(int power_start)
{
	uint8_t val;
	struct axp_dev *axp;
	axp = axp_dev_lookup(AXP259);
	if (NULL == axp) {
		printk(KERN_ERR"%s: axp data is null\n", __func__);
		return;
	}

	printk(KERN_DEBUG"[axp] send power-off command!\n");

	mdelay(20);

	if (power_start != 1) {
		axp_read(axp->dev, AXP259_STATUS, &val);
		if ((val & 0x11) == 0x11) {
			printk(KERN_DEBUG"[axp] set flag!\n");
			if (0x0d != val)
				axp_write(axp->dev, AXP259_BUFFERB, 0x0f);
			mdelay(20);
			printk(KERN_DEBUG"[axp] reboot!\n");
			machine_restart(NULL);
			printk(KERN_ERR"[axp] warning!!! arch can't ,reboot, maybe some error happend!\n");
		}
	}
	axp_read(axp->dev, AXP259_BUFFERB, &val);
	if (0x0d != val)
		axp_write(axp->dev, AXP259_BUFFERB, 0x00);

	axp_set_bits(axp->dev, AXP259_OFF_CTL, 0x40);
	mdelay(40);
	printk(KERN_ERR"[axp] warning!!! axp can't power-off, maybe some error happend!\n");

}


#ifdef	CONFIG_AXP_TWI_USED
static const struct i2c_device_id axp_i2c_id_table[] = {
	{ "axp259_board", 0 },
	{},
};
MODULE_DEVICE_TABLE(i2c, axp_i2c_id_table);

static struct i2c_board_info __initdata axp_mfd_i2c_board_info[] = {
	{
		.type = "axp259_board",
		.addr = 0x36,
	},
};

static irqreturn_t axp_mfd_irq_handler(int irq, void *data)
{
	struct axp_dev *chip = data;
	disable_irq_nosync(irq);
	(void)schedule_work(&chip->irq_work);

	return IRQ_HANDLED;
}

static int axp_i2c_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	int ret = 0;
	printk(KERN_ERR"[%s] line=%d\n", __func__, __LINE__);
	axp259_dev.client = client;
	axp259_dev.dev = &client->dev;

	i2c_set_clientdata(client, &axp259_dev);

	axp259_dev.ops = &axp259_ops[0];
	axp259_dev.attrs = axp259_mfd_attrs;
	axp259_dev.attrs_number = ARRAY_SIZE(axp259_mfd_attrs);
	axp259_dev.pdata = &axp_pdata;
	ret = axp_register_mfd(&axp259_dev);
	if (ret < 0) {
		printk(KERN_ERR"axp mfd register failed\n");
		return ret;
	}

	ret = request_irq(client->irq, axp_mfd_irq_handler,
		IRQF_SHARED|IRQF_DISABLED, "axp259", &axp259_dev);
	if (ret) {
		dev_err(&client->dev, "failed to request irq %d\n",
				client->irq);
		goto out_free_chip;
	}

	return ret;


out_free_chip:
	i2c_set_clientdata(client, NULL);
	return ret;
}

static int axp_i2c_remove(struct i2c_client *client)
{
	return 0;
}

static struct i2c_driver axp_i2c_driver = {
	.driver	= {
		.name	= "axp259_board",
		.owner	= THIS_MODULE,
	},
	.probe		= axp_i2c_probe,
	.remove		= axp_i2c_remove,
	.id_table	= axp_i2c_id_table,
};
#else
static int  axp259_platform_probe(struct platform_device *pdev)
{
	int ret = 0;

	axp259_dev.dev = &pdev->dev;
	dev_set_drvdata(axp259_dev.dev, &axp259_dev);
	axp259_dev.client = (struct i2c_client *)pdev;

	axp259_dev.ops = &axp259_ops[0];
	axp259_dev.attrs = axp259_mfd_attrs;
	axp259_dev.attrs_number = ARRAY_SIZE(axp259_mfd_attrs);
	axp259_dev.pdata = &axp_pdata;
	ret = axp_register_mfd(&axp259_dev);
	if (ret < 0) {
		printk(KERN_ERR"axp259 mfd register failed\n");
		return ret;
	}
	return 0;
}

static struct platform_device axp259_platform_device = {
	.name		    = "axp259_board",
	.id		        = PLATFORM_DEVID_NONE,
};

static struct platform_driver axp259_platform_driver = {
	.probe		= axp259_platform_probe,
	.driver		= {
		.name	= "axp259_board",
		.owner	= THIS_MODULE,
	},
};
#endif

static int __init axp259_board_init(void)
{
	int ret = 0;

	ret = axp_fetch_sysconfig_para("pmu1_para", &axp259_config);
	if (ret) {
		printk(KERN_ERR"%s fetch sysconfig err", __func__);
		return -1;
	}
	if (axp259_config.pmu_used) {
		battery_data.voltage_max_design = axp259_config.pmu_init_chgvol;
		battery_data.voltage_min_design = axp259_config.pmu_pwroff_vol;
		battery_data.energy_full_design = axp259_config.pmu_battery_cap;
		axp_sply_init_data.chgcur = axp259_config.pmu_runtime_chgcur;
		axp_sply_init_data.chgvol = axp259_config.pmu_init_chgvol;
		axp_sply_init_data.chgend = axp259_config.pmu_init_chgend_rate;
		axp_sply_init_data.chgen = axp259_config.pmu_init_chg_enabled;
		axp_sply_init_data.sample_time =
			axp259_config.pmu_init_adc_freq;
		axp_sply_init_data.chgpretime =
			axp259_config.pmu_init_chg_pretime;
		axp_sply_init_data.chgcsttime =
			axp259_config.pmu_init_chg_csttime;
#ifdef	CONFIG_AXP_TWI_USED
		axp_mfd_i2c_board_info[0].addr = axp259_config.pmu_twi_addr;
		axp_mfd_i2c_board_info[0].irq = axp259_config.pmu_irq_id;
		printk(KERN_ERR"twi addr=0x%x, irq_id=%d\n",
			axp_mfd_i2c_board_info[0].addr,
			axp_mfd_i2c_board_info[0].irq);
		ret = i2c_add_driver(&axp_i2c_driver);
		if (ret < 0) {
			printk(KERN_ERR"axp_i2c_driver add failed\n");
			return ret;
		}

		ret = i2c_register_board_info(axp259_config.pmu_twi_id,
			axp_mfd_i2c_board_info,
			ARRAY_SIZE(axp_mfd_i2c_board_info));
		if (ret < 0) {
			printk(KERN_ERR"axp_i2c_board_info add failed\n");
			return ret;
		}
#else
		ret = platform_driver_register(&axp259_platform_driver);
		if (IS_ERR_VALUE(ret)) {
			printk(KERN_ERR"register axp259 platform driver failed\n");
			return ret;
		}
		ret = platform_device_register(&axp259_platform_device);
		if (IS_ERR_VALUE(ret)) {
			printk(KERN_ERR"register axp259 platform device failed\n");
			return ret;
		}
#endif
	}
	return ret;
}

static void __exit axp259_board_exit(void)
{

}

#ifdef	CONFIG_AXP_TWI_USED
arch_initcall(axp259_board_init);
#else
subsys_initcall(axp259_board_init);
#endif

module_exit(axp259_board_exit);

MODULE_DESCRIPTION("AXP259 board");
MODULE_AUTHOR("Forever Cai");
MODULE_LICENSE("GPL");
