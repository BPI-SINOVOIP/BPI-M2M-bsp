/*
 * Platform interfaces for XRadio drivers
 *
 * Implemented by platform vendor(such as AllwinnerTech).
 *
 * Copyright (c) 2013, XRadio
 * Author: XRadio
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/delay.h>

#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/ioport.h>

#include <linux/regulator/consumer.h>
#include <asm/mach-types.h>
#include <mach/sys_config.h>
#include <linux/pinctrl/pinconf-sunxi.h>
//#include <linux/sys_config.h>

#include <linux/of.h>
#include<linux/of_address.h>
#include<linux/of_irq.h>
#include<linux/of_gpio.h>

#include "xradio.h"
#include "platform.h"
#include "sbus.h"

#define PLATFORM_RFKILL_PM     0  //if 1, Use system rf-kill for wifi power manage, default 0.
#define PLATFORM_SYSCONFIG     1  //if 1, Get Hardware settting from sysconfig.fex, default 1.
#define PMU_POWER_WLAN_RETAIN  1  //if 0, PMU power down when wifi is closed, default 1.

#define WIFI_CONFIG        "wifi_para"
#define MAX_POWER_NUM      3
static int wlan_bus_id     = 0;
static u32 wlan_irq_gpio   = 0;
static u32 wlan_reset_gpio = 0;
static u32 wlan_LED0 = 0;
static u32 wlan_LED1 = 0;


static char * axp_name[MAX_POWER_NUM]  = {0};
static u32 gpio_irq_handle = 0;
static u32 gpio_irq_num=0;
#if (!PLATFORM_SYSCONFIG)
//--------------------------------------------------
//Select one platform only, default is EVB_A31 card1.
//--------------------------------------------------
//#define  V819MINI
//#define  HP8_G2
//#define  EVB_A31_CARD0
//#define  EVB_A31_CARD1
//#define  EVB_A83T_ONBOARD
//#define  EVB_A33_XR819
//#define  A33_CHIPHD

#define  V3s_CHIPHD 1

//--------------------------------------------------
// sdio and gpio settings of platforms
//--------------------------------------------------
//V819MINI
#ifdef   V819MINI
#define  PLAT_GPIO_WRESET    (GPIOM(7))
#define  PLAT_GPIO_WLAN_INT  (GPIOL(6))
#define  PLAT_MMC_ID		  1

//HP8 G2
#elif defined(HP8_G2)
#define  PLAT_GPIO_WRESET    (GPIOL(6))
#define  PLAT_GPIO_WLAN_INT  (GPIOL(7))
#define  PLAT_MMC_ID          1

//EVB_A83T_ONBOARD
#elif defined(EVB_A83T_ONBOARD)
#define  PLAT_GPIO_WRESET    (GPIOL(9))
#define  PLAT_GPIO_WLAN_INT  (GPIOL(11))
#define  PLAT_MMC_ID          1

//EVB_A33_XR819_test
#elif defined(EVB_A33_XR819)
#define  PLAT_GPIO_WRESET    (GPIOL(11))
#define  PLAT_GPIO_WLAN_INT  (GPIOL(8))
#define  PLAT_MMC_ID          1

//A33_CHIPHD
#elif defined(A33_CHIPHD)
#define  PLAT_GPIO_WRESET    (GPIOL(6))
#define  PLAT_GPIO_WLAN_INT  (GPIOE(12))
#define  PLAT_MMC_ID          1

#elif defined(C500_CHIPHD)
#define  PLAT_GPIO_WRESET    (GPIOD(10))
#define  PLAT_GPIO_WLAN_INT  (GPIOD(11))
#define  PLAT_MMC_ID          0

//defualt EVB_A31_CARD0.
#elif defined(EVB_A31_CARD0)
#define  PLAT_GPIO_WRESET    (GPIOM(4))
#define  PLAT_GPIO_WLAN_INT  (GPIOM(6))
#define  PLAT_MMC_ID          0

#elif defined(V3s_CHIPHD)
#define  PLAT_GPIO_WRESET    (GPIOE(23))
#define  PLAT_GPIO_WLAN_INT  (GPIOB(5))
#define  PLAT_MMC_ID          1


#else  //defualt EVB_A31_CARD1.
#define  PLAT_GPIO_WRESET    (GPIOM(4))
#define  PLAT_GPIO_WLAN_INT  (GPIOM(6))
#define  PLAT_MMC_ID          1

#endif //end of V819MINI

#endif //PLATFORM_SYSCONFIG
static int xradio_set_gpio_val(u32 gpio, u32 val);


/*********************Interfaces on Allwinner platform. *********************/
#if (!PLATFORM_RFKILL_PM)
static int plat_evb_a31_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int	clk;
	int val;
	int ret = 0;

	if (!on) return ret; //no need power down on evb_a31.

#ifdef EVB_A31_CARD0
	printk(KERN_ERR "evb card0 (red card) present!.\n");
  //set sd card 0 to 1.8v
	ldo = regulator_get(NULL, "axp22_eldo2");
	if(ldo) {
		//set new voltage 1.8v
		val = 1800000;
		if(regulator_set_voltage(ldo, val, val) < 0)
			printk(KERN_ERR "regulator_set_voltage failed.\n");
		//enable 	regulator
		ret = regulator_enable(ldo);
		if(ret < 0){
			printk(KERN_ERR "regulator_enable failed.\n");
		}
		//check voltage
		val = regulator_get_voltage(ldo);
		printk(KERN_ERR "sdio_vol(eldo2)=%duV.\n", val);
	} else {
		printk(KERN_ERR "regulator get eldo2 failed.\n");
	}
	regulator_put(ldo);

	//check pm_io voltage
	ldo = regulator_get(NULL, "axp22_aldo2");
	if(ldo) {
		//try to set 1.8v to pm voltage
		val = 1800000;
		regulator_set_voltage(ldo, val, val);
		regulator_enable(ldo);

		val = regulator_get_voltage(ldo);
		printk(KERN_ERR "pm_io_vol(aldo2)=%duV.\n", val);
	} else {
		printk(KERN_ERR "regulator get aldo2 failed.\n");
	}
	regulator_put(ldo);

#else
	printk(KERN_ERR "evb card1 (green card) present!.\n");

	//check pm_io voltage
	ldo = regulator_get(NULL, "axp22_aldo2");
	if(ldo) {
		//try to set 1.8v to pm voltage
		val = 1800000;
		regulator_set_voltage(ldo, val, val);
		regulator_enable(ldo);

		val = regulator_get_voltage(ldo);
		printk(KERN_ERR "pm_io_vol(aldo2)=%duV.\n", val);
	} else {
		printk(KERN_ERR "regulator get aldo2 failed.\n");
	}
	regulator_put(ldo);

		/* pm7配置为rtc_clk0 */
	clk = ioread32(0xf1f02c24);
	clk &= ~(0x3 << 28);
	clk |= 0x3 << 28;
	iowrite32(clk, 0xf1f02c24);

	/* losc配成external 32.768KHz */
	clk = ioread32(0xf1f00004);
	clk &= ~0x1;
	clk |= 0x1;
	iowrite32(clk, 0xf1f00004);

	/* clk_outputd使能 */
	clk = ioread32(0xf1f014f0);
	clk = 0x80000000;
	iowrite32(clk, 0xf1f014f0);

	printk(KERN_ERR "[clock] clock init RTC-CKO 32768Hz," \
	           " 0xf1f02c24=%#x, 0xf1f00004=%#x, 0xf1f014f0=%#x\n",
	           ioread32(0xf1f02c24), ioread32(0xf1f00004), ioread32(0xf1f014f0));

	gpio_request(GPIOM(3), "pmu_en");
	gpio_direction_output(GPIOM(3), 0);
	gpio_set_value(GPIOM(3), 1);
	udelay(300);

	gpio_request(GPIOG(12), "gbf_ena_reset");
	gpio_direction_output(GPIOG(12), 0);
	gpio_set_value(GPIOG(12), 1);
	udelay(300);

	gpio_free(GPIOM(3));
	gpio_free(GPIOG(12));
#endif //EVB_CARD0

	return ret;
}

static int plat_v819mini_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int gpio;
	unsigned int	clk;
	int ret = 0;

	xradio_dbg(XRADIO_DBG_ERROR, "V819mini Power %s!.\n", on? "Up":"Down");

	if (on) {
		//wifi vcc 3.3v for XR819
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		//pg sdio pin 3.3v for XR819
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi reset 3.3v for XR819
		ldo = regulator_get(NULL, "axp22_dldo4");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo4=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
	} else {
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
	}

  return ret;
}

static int plat_hp8_g2_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int gpio;
	unsigned int	clk;
	int ret = 0;

	printk(KERN_ERR "hp8_g2 Power %s!.\n", on? "Up":"Down");

	if (on) {
		//wifi vcc 3.3v for XR819.
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi vcc 1.8v for XR819.
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_eldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable eldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi io pin 1.8~3.3v for XR819.
		ldo = regulator_get(NULL, "axp22_aldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 3000000, 3000000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable aldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		//wifi reset & interrupt 1.8~3.3v for XR819.
	} else {
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_eldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
	}

	return ret;
}

static int plat_evb_a83t_onboard_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int gpio;
	unsigned int	clk;
	int ret = 0;
	printk(KERN_ERR "evb_a83t_onboard Power %s!.\n", on? "Up":"Down");
	if (on) {
		//wifi vcc 3.3v for XR819.
		ldo = regulator_get(NULL, "axp81x_dldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi vcc 1.8v for XR819.
		ldo = regulator_get(NULL, "axp81x_aldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable aldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi io pin 1.8~3.3v for XR819b.
		ldo = regulator_get(NULL, "axp81x_aldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 3000000, 3000000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable aldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi reset & interrupt 1.8~3.3v for XR819.
		ldo = regulator_get(NULL, "axp81x_aldo3");
		if (ldo) {
			regulator_set_voltage(ldo, 3000000, 3000000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable aldo3=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
	} else {
		ldo = regulator_get(NULL, "axp81x_dldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp81x_aldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
	}

	return ret;
}

static int plat_a33_xr819_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int gpio;
	unsigned int	clk;
	int ret = 0;

	printk(KERN_ERR "plat_a33_xr819_pwr Power %s!.\n", on? "Up":"Down");
	if (on) {
		//wifi vcc 3.3v for XR819.
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo4");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo4=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi vcc 1.8v for XR819.
		ldo = regulator_get(NULL, "axp22_eldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable eldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_eldo3");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable eldo3=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		//wifi reset & interrupt 1.8~3.3v for XR819.
	} else {
		//wifi vcc 3.3v
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo4");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		//wifi vcc 1.8v
		ldo = regulator_get(NULL, "axp22_eldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_eldo3");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
	}
	return ret;
}
static int plat_c500_chiphd_pwr(int on)
{
	printk(KERN_ERR "plat_C500_chiphd_pwr Power %s!.\n", on? "Up":"Down");
}
static int plat_a33_chiphd_pwr(int on)
{
	struct regulator *ldo = NULL;
	unsigned int gpio;
	unsigned int	clk;
	int ret = 0;
	struct clk *ap_32k = NULL;

	printk(KERN_ERR "plat_a33_chiphd_pwr Power %s!.\n", on? "Up":"Down");
	if (on) {
		//wifi vcc 3.3v for XR819.
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			regulator_set_voltage(ldo, 3300000, 3300000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable dldo2=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}

		//wifi vcc 1.8v for XR819.
		ldo = regulator_get(NULL, "axp22_eldo1");
		if (ldo) {
			regulator_set_voltage(ldo, 1800000, 1800000);
			ret = regulator_enable(ldo);
			if(ret < 0){
				printk(KERN_ERR "regulator_enable failed.\n");
			}
			printk(KERN_ERR "regulator_enable eldo1=%d.\n", regulator_get_voltage(ldo));
			regulator_put(ldo);
		}
		//wifi reset & interrupt 1.8~3.3v for XR819.
	} else {
		//wifi vcc 3.3v
		ldo = regulator_get(NULL, "axp22_dldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		ldo = regulator_get(NULL, "axp22_dldo2");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
		//wifi vcc 1.8v
		ldo = regulator_get(NULL, "axp22_eldo1");
		if (ldo) {
			ret = regulator_disable(ldo);
			regulator_put(ldo);
		}
	}

	//disable 32k clk.
	ap_32k = clk_get(NULL, "losc_out");
	if (!ap_32k){
		printk(KERN_ERR "Get ap 32k clk out failed!\n");
		return ;
	}
	clk_disable(ap_32k);
	//ret = clk_prepare_enable(ap_32k);
	//if (ret){
	//	printk(KERN_ERR "losc out enable failed!\n");
	//}

	return ret;
}
#endif //PLATFORM_RFKILL_PM


int plat_power(int enable)
{
	int ret = 0;

#ifdef   V819MINI
	ret = plat_v819mini_pwr(enable);
#elif defined(HP8_G2)
	ret = plat_hp8_g2_pwr(enable);
#elif defined(EVB_A83T_ONBOARD)
	ret = plat_evb_a83t_onboard_pwr(enable);
#elif defined(EVB_A33_XR819)
	ret = plat_a33_xr819_pwr(enable);
#elif defined(A33_CHIPHD)
	ret = plat_a33_chiphd_pwr(enable);
#elif (defined(EVB_A31_CARD0)||defined(EVB_A31_CARD1))
	ret = plat_evb_a31_pwr(enable);
#elif (defined(C500_CHIPHD))
	plat_c500_chiphd_pwr(enable);
#else
	printk(KERN_ERR "plat_power: Fail to setup power!\n");
#endif

	if (enable) {  //delay for power stable period.
		mdelay(50);
	}
	return ret;
}

/*********************Interfaces called by xradio core. *********************/
#if (PLATFORM_SYSCONFIG)
static int xradio_get_syscfg(void)
{
	script_item_u val;
	script_item_value_type_e type;

	/* Get SDIO/USB config. */
#if defined(CONFIG_XRADIO_SDIO)
	type = script_get_item(WIFI_CONFIG, "wifi_sdc_id", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		printk(KERN_ERR "failed to fetch sdio card's sdcid\n");
		return -1;
	}
#elif defined(CONFIG_XRADIO_USB)
	type = script_get_item(WIFI_CONFIG, "wifi_usbc_id", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		printk(KERN_ERR "failed to fetch usb's id\n");
		return -1;
	}
#endif
	wlan_bus_id = val.val;

	/* Get GPIO config. */
	type = script_get_item(WIFI_CONFIG, "xradio_wl_regon", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "failed to fetch xradio_wl_regon\n");
		return -1;
	}
	wlan_reset_gpio = val.gpio.gpio;

	type = script_get_item(WIFI_CONFIG, "xradio_wl_host_wake", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "failed to fetch xradio_wl_host_wake\n");
		return -1;
	}
	wlan_irq_gpio = val.gpio.gpio;
#if 0
	/* Get Power config. */
	type = script_get_item(WIFI_CONFIG, "wifi_power", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_STR == type) {
		axp_name[0] = val.str;
		printk("wifi_power=%s\n", axp_name[0]);
	} else {
		printk(KERN_ERR "wifi_power not config\n");
	}
	type = script_get_item(WIFI_CONFIG, "wifi_power_ext1", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_STR == type) {
		axp_name[1] = val.str;
		printk("wifi_power_ext1=%s\n", axp_name[1]);
	} else {
		printk(KERN_ERR "wifi_power_ext1 not config\n");
	}
	type = script_get_item(WIFI_CONFIG, "wifi_power_ext2", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_STR == type) {
		axp_name[2] = val.str;
		printk("wifi_power_ext2=%s\n", axp_name[2]);
	} else {
		printk(KERN_ERR "wifi_power_ext2 not config\n");
	}
#endif
	return 0;
}
#endif

#ifdef CONFIG_OF
#define TYPE_INTEGER	1
#define TYPE_STRING		2
#define TYPE_GPIO		3
#define TYPE_EXT_IRQ	4

typedef struct
{
	char  gpio_name[32];
	int port;
	int port_num;
	int mul_sel;
	int pull;
	int drv_level;
	int data;
	int gpio;
} wifi_gpio_set_t;

int xradio_get_wifi_para(char *main_name, char *sub_name, int value[], int type)
{
	char compat[32];
	u32 len = 0;
	struct device_node *node;
	struct device_node *node_parent;
	int ret = 0;
	struct gpio_config config;
/*
	node_parent = of_find_node_by_type(NULL, "soc");
	if (!node_parent) {
		printk(KERN_WARNING "of_find_node_by_name soc fail\n");
		return ret;
	}
*/
	node = of_find_node_by_type(NULL, "wifi_conf");
	if (!node) {
		printk(KERN_WARNING "of_find_node_by_name %s fail\n", "wifi_conf");
		return ret;
	}

	if (TYPE_INTEGER == type) {			/*integer*/
		if (of_property_read_u32_array(node, sub_name, value, 1))
			pr_info("of_property_read_u32_array %s.%s fail\n", main_name, sub_name);
		else
			ret = type;
	} else if (TYPE_STRING == type) {	/*string*/
		const char *str;

		if (of_property_read_string(node, sub_name, &str))
			pr_info("of_property_read_string %s.%s fail\n", main_name, sub_name);
		else {
			ret = type;
			memcpy((void*)value, str, strlen(str)+1);
		}
	} else if (TYPE_GPIO == type) {	/*gpio*/
		wifi_gpio_set_t *gpio_info = (wifi_gpio_set_t *)value;
		int gpio;

		gpio = of_get_named_gpio_flags(node, sub_name, 0, (enum of_gpio_flags *)&config);
		if (!gpio_is_valid(gpio))
			goto exit;

		gpio_info->gpio = config.gpio;
		gpio_info->mul_sel = config.mul_sel;
		gpio_info->pull = config.pull;
		gpio_info->drv_level = config.drv_level;
		gpio_info->data = config.data;
		memcpy(gpio_info->gpio_name, sub_name, strlen(sub_name)+1);
		printk(KERN_INFO "%s.%s gpio=%d,mul_sel=%d,data:%d\n",main_name, sub_name, gpio_info->gpio, gpio_info->mul_sel, gpio_info->data);
		ret = type;
	} else if (TYPE_EXT_IRQ == type) {
		*value = irq_of_parse_and_map(node, 0);
		ret = type;
	}

exit:
	return ret;
}

static int xradio_para_wifi_config(void)
{
	int ret = -1;
	int value = 0;
	wifi_gpio_set_t pin;

	ret = xradio_get_wifi_para(NULL, NULL, &value, TYPE_EXT_IRQ);
	if(TYPE_EXT_IRQ != ret) {
		printk(KERN_WARNING "%s: fail to get wifi irq num\n", __func__);
		gpio_irq_num = 0;
	} else {
		gpio_irq_num = value;
		printk(KERN_INFO "%s: wifi irq num=%d\n", __func__, gpio_irq_num);
	}

	ret = xradio_get_wifi_para(NULL, "reset_pin", &pin, TYPE_GPIO);
	if(TYPE_GPIO != ret) {
		printk(KERN_WARNING "%s: fail to get wifi reset pin\n", __func__);
		wlan_reset_gpio = 0;
	} else {
		wlan_reset_gpio = pin.gpio;
		printk(KERN_INFO "%s: wifi reset_pin gpio num=%d\n", __func__, wlan_reset_gpio);
	}

	ret = xradio_get_wifi_para(NULL, "irq_pin", &pin, TYPE_GPIO);
	if(TYPE_GPIO != ret) {
		printk(KERN_WARNING "%s: fail to get wifi irq pin\n", __func__);
		wlan_irq_gpio = 0;
	} else {
		wlan_irq_gpio = pin.gpio;
		printk(KERN_INFO "%s: wifi irq_pin gpio num=%d\n", __func__, wlan_irq_gpio);
	}
	ret = xradio_get_wifi_para(NULL, "LED0_pin", &pin, TYPE_GPIO);
	if(TYPE_GPIO != ret) {
		printk(KERN_WARNING "%s: fail to get wifi LED0 pin\n", __func__);
		wlan_LED0 = 0;
	} else {
		wlan_LED0 = pin.gpio;
		printk(KERN_INFO "%s: wifi LED0 gpio num=%d\n", __func__, wlan_irq_gpio);
	}
	ret = xradio_get_wifi_para(NULL, "LED1_pin", &pin, TYPE_GPIO);
	if(TYPE_GPIO != ret) {
		printk(KERN_WARNING "%s: fail to get wifi LED1 pin\n", __func__);
		wlan_LED1 = 0;
	} else {
		wlan_LED1 = pin.gpio;
		printk(KERN_INFO "%s: wifi LED1 gpio num=%d\n", __func__, wlan_irq_gpio);
	}

	return 0;
}
#endif

int  xradio_plat_init(void)
{
	int ret = 0;
#if (!PLATFORM_SYSCONFIG)
	wlan_bus_id     = PLAT_MMC_ID;
	wlan_irq_gpio   = PLAT_GPIO_WLAN_INT;
	wlan_reset_gpio = PLAT_GPIO_WRESET;
#else  //initialize from sys config.
	ret = xradio_get_syscfg();
//	ret = xradio_para_wifi_config();
	if(wlan_LED0)
		xradio_set_gpio_val(wlan_LED0,0);
	if(wlan_LED1)
		xradio_set_gpio_val(wlan_LED1,0);

#endif  //PLATFORM_SYSCONFIG

#if (0)
#if (PMU_POWER_WLAN_RETAIN && !PLATFORM_RFKILL_PM)
	plat_power(1);
#endif
#endif
	return ret;
}

void xradio_plat_deinit(void)
{
#if (0)
#if (PMU_POWER_WLAN_RETAIN && !PLATFORM_RFKILL_PM)
	plat_power(0);
#endif
#endif
}

#if (PLATFORM_RFKILL_PM)
extern void wifi_pm_power(int on);
#endif
extern int pin_config_set(const char *dev_name, const char *name, unsigned long config);
static int  xradio_request_gpio(struct gpio_config *pinConfig,__u32 group_count_max)
{
	struct gpio_config pin_cfg;
	char   pin_name[32];
	int ret ;
	u32 config;

	pr_info("%s: test1\n", __func__);
	pin_cfg.gpio = pinConfig->gpio;
	pin_cfg.mul_sel = pinConfig->mul_sel;
	pin_cfg.pull = pinConfig->pull;
	pin_cfg.drv_level = pinConfig->drv_level;
	pin_cfg.data = pinConfig->data;

	pr_info("%s: request gpio=%d\n", __func__, pin_cfg.gpio);
	ret = gpio_request(pin_cfg.gpio, NULL);
    if (0 == ret) {
		sunxi_gpio_to_name(pin_cfg.gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg.mul_sel);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (pin_cfg.pull != GPIO_PULL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, pin_cfg.pull);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.drv_level != GPIO_DRVLVL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DRV, pin_cfg.drv_level);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.data != GPIO_DATA_DEFAULT) {
			pr_info("%s: L%d Pin=%d\n", __func__, __LINE__, pin_cfg.data);
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, pin_cfg.data);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
    }

	pr_info("%s: request gpio(%d) %s\n", __func__, pin_cfg.gpio, ((ret?"fail":"ok")));
	return ret;

}

static int xradio_set_gpio_val(u32 gpio, u32 val)
{
	struct gpio_config pin_cfg;
	int ret = -1;

	pin_cfg.gpio=gpio;
	pin_cfg.drv_level=1;//SUNXI_DRV_LEVEL1;
	pin_cfg.mul_sel=1;//OUTPUT
	pin_cfg.data=((val)?1:0);
	pin_cfg.pull=1;

	ret = xradio_request_gpio(&pin_cfg,0);
	if (0 == ret)
		gpio_free(gpio);
	return ret;
}

int xradio_wlan_power(int on)
{
	int ret = 0;
	struct gpio_config pin_cfg;

#if (PLATFORM_RFKILL_PM)
	wifi_pm_power(on);
	mdelay(50);
#else //PLATFORM_RFKILL_PM
	ret = xradio_set_gpio_val(wlan_reset_gpio, 0);
	pr_info("%s ret=%d on=%d\n", __func__, ret, on);
	if (!ret) {
		if (on) {  //power up.
#if (0)
#if (!PMU_POWER_WLAN_RETAIN)
			plat_power(1);
			mdelay(50);
#endif
#endif
			xradio_set_gpio_val(wlan_reset_gpio, 1);
			mdelay(50);
			xradio_set_gpio_val(wlan_reset_gpio, 0);
			mdelay(2);
			xradio_set_gpio_val(wlan_reset_gpio, 1);
			mdelay(50);
		} else { //power down.
			xradio_set_gpio_val(wlan_reset_gpio, 0);
#if (0)
#if (!PMU_POWER_WLAN_RETAIN)
			plat_power(0);
#endif
#endif
		}
		gpio_free(wlan_reset_gpio);
	} else {
		xradio_dbg(XRADIO_DBG_ERROR, "%s: gpio_request err: %d\n", __func__, ret);
	}
#endif
	return ret;
}

int xradio_sdio_detect(int enable)
{
	int insert = enable;
	MCI_RESCAN_CARD(wlan_bus_id, insert);
	xradio_dbg(XRADIO_DBG_ALWY, "%s SDIO card %d\n",
	           enable?"Detect":"Remove", wlan_bus_id);
	mdelay(10);
}

#ifdef CONFIG_XRADIO_USE_GPIO_IRQ

#ifdef PLAT_ALLWINNER_SUNXI
static irqreturn_t xradio_gpio_irq_handler(int irq, void *sbus_priv)
{
	struct sbus_priv *self = (struct sbus_priv *)sbus_priv;
	unsigned long flags;

	SYS_BUG(!self);
	spin_lock_irqsave(&self->lock, flags);
	if (self->irq_handler)
		self->irq_handler(self->irq_priv);
	spin_unlock_irqrestore(&self->lock, flags);
	return IRQ_HANDLED;
}
#else //PLAT_ALLWINNER_SUN6I
static u32 xradio_gpio_irq_handler(void *sbus_priv)
{
	struct sbus_priv *self = (struct sbus_priv *)sbus_priv;
	unsigned long flags;

	SYS_BUG(!self);
	spin_lock_irqsave(&self->lock, flags);
	if (self->irq_handler)
		self->irq_handler(self->irq_priv);
	spin_unlock_irqrestore(&self->lock, flags);
	return 0;
}
#endif

int xradio_request_gpio_irq(struct device *dev, void *sbus_priv)
{
	struct gpio_config pin_cfg;
	int ret = -1;
	if(!gpio_irq_handle) {

#ifdef PLAT_ALLWINNER_SUNXI
		pin_cfg.gpio=wlan_irq_gpio;
		pin_cfg.drv_level=1;//SUNXI_DRV_LEVEL1;
		pin_cfg.mul_sel=6;//
		pin_cfg.data=1;
		pin_cfg.pull=1;
		//ret =xradio_request_gpio(&pin_cfg,0);
		gpio_irq_num=gpio_to_irq(pin_cfg.gpio);


		ret = request_irq(gpio_irq_num,
		                      (irq_handler_t)xradio_gpio_irq_handler,
		                       IRQF_TRIGGER_RISING, "xradio_irq", sbus_priv);
		if (IS_ERR_VALUE(ret)) {

			gpio_irq_handle = 0;

		}
		else
			gpio_irq_handle=1;


#else//PLAT_ALLWINNER_SUN6I
		gpio_irq_handle = sw_gpio_irq_request(wlan_irq_gpio, TRIG_EDGE_POSITIVE,
			                                        (peint_handle)xradio_gpio_irq_handler, sbus_priv);
#endif // PLAT_ALLWINNER_SUNXI
	} else {
		xradio_dbg(XRADIO_DBG_ERROR, "%s: error, irq exist already!\n", __func__);
	}

	if (gpio_irq_handle) {
		xradio_dbg(XRADIO_DBG_NIY, "%s: request_irq sucess! irq=0x%08x\n",
		           __func__, gpio_irq_handle);
		ret = 0;
	} else {
		xradio_dbg(XRADIO_DBG_ERROR, "%s: request_irq err: %d\n", __func__, ret);
		ret = -1;
	}
	return ret;
}

int xradio_free_gpio_irq(struct device *dev, void *sbus_priv)
{
	struct sbus_priv *self = (struct sbus_priv *)sbus_priv;
#ifdef PLAT_ALLWINNER_SUNXI
	if(gpio_irq_handle) {
		//for linux3.4
		free_irq(gpio_irq_num, self);
		gpio_free(wlan_irq_gpio);
		gpio_irq_handle = 0;
	}
#else// PLAT_ALLWINNER_SUN6I
	if(gpio_irq_handle) {
		sw_gpio_irq_free(gpio_irq_handle);
		gpio_irq_handle = 0;
	}
#endif//PLAT_ALLWINNER_SUNXI
}
#endif /* CONFIG_XRADIO_USE_GPIO_IRQ */

/******************************************************************************************/
