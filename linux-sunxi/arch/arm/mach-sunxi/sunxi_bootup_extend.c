/*
 * linux/arch/arm/mach-sunxi/sunxi_bootup_extend.c
 *
 * Copyright(c) 2013-2015 Allwinnertech Co., Ltd.
 *         http://www.allwinnertech.com
 *
 * allwinner sunxi platform bootup extend code.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define pr_fmt(fmt) "sunxi_bootup_extend: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/reboot.h>

#include <asm/mcpm.h>
#include <asm/proc-fns.h>
#include <asm/cacheflush.h>
#include <asm/cputype.h>
#include <asm/cp15.h>
#include <asm/smp_plat.h>
#include <asm/hardware/gic.h>
#include <linux/arisc/hwspinlock.h>

#include <mach/platform.h>
#include <mach/sys_config.h>
#include <mach/sunxi-chip.h>

/* rtc reboot flags: sync from boot */
#define SUNXI_DEBUG_MODE_FLAG           (0x59)    /* debug */
#define SUNXI_EFEX_CMD_FLAG             (0x5A)    /* efex */
#define SUNXI_BOOT_RESIGNATURE_FLAG     (0x5B)    /* boot-resignature */
#define SUNXI_BOOT_RECOVERY_FLAG        (0x5C)    /* recovery or boot-recovery */
#define SUNXI_SYS_RECOVERY_FLAG         (0x5D)    /* sysrecovery */
#define SUNXI_USB_RECOVERY_FLAG         (0x5E)    /* usb-recovery*/
#define SUNXI_FASTBOOT_FLAG             (0x5F)    /* bootloader */

/* box fake shutdown flags: sync from cpus */
#define SUNXI_BOX_POWEROFF_FLAG         (0x01)
#define SUNXI_BOX_RESTART_FLAG          (0x0f)

/* rtc register config */
#if (defined CONFIG_ARCH_SUN8IW6P1) || (defined CONFIG_ARCH_SUN9IW1P1)
#define SUNXI_RTC_GPREG_BASE              (SUNXI_R_PRCM_VBASE + 0x1F0)
#else
#define SUNXI_RTC_GPREG_BASE              (SUNXI_RTC_VBASE + 0x100)
#endif
#define SUNXI_RTC_GPREG_NUM               0x03

#ifdef CONFIG_SUNXI_ARISC
static DEFINE_SPINLOCK(rtc_hw_lock);

static int rtc_hwspin_lock_timeout(unsigned int timeout, unsigned long *flags)
{
	return arisc_hwspin_lock_timeout(AW_RTC_REG_HWSPINLOCK, timeout, &rtc_hw_lock, flags);
}

static int rtc_hwspin_unlock(unsigned long *flags)
{
	return arisc_hwspin_unlock(AW_RTC_REG_HWSPINLOCK, &rtc_hw_lock, flags);
}
#else
static int rtc_hwspin_lock_timeout(unsigned int timeout, unsigned long *flags)
{
	return 0;
}

static int rtc_hwspin_unlock(unsigned long *flags)
{
	return 0;
}
#endif

static int rtc_gpreg_write(unsigned int regnum, unsigned int data)
{
	unsigned long hwflags;

	if (!rtc_hwspin_lock_timeout(100, &hwflags)) {
#if (defined CONFIG_ARCH_SUN8IW6P1) || (defined CONFIG_ARCH_SUN9IW1P1)
		unsigned int temp = 0;
		temp = (regnum << 16) | (data << 8);
		writel(temp, SUNXI_RTC_GPREG_BASE);
		temp |= 1 << 31;
		writel(temp, SUNXI_RTC_GPREG_BASE);
		temp &= ~(1 << 31);
		writel(temp, SUNXI_RTC_GPREG_BASE);
#else
		writel(data, SUNXI_RTC_GPREG_BASE + 0x4 * regnum);
#endif

		rtc_hwspin_unlock(&hwflags);
		pr_debug("write rtc gp reg 0x%x 0x%x\n", regnum, data);
		return 0;
	}

	pr_debug("get hwspinlock unsuccess\n");
	return -1;
}

#ifdef CONFIG_SUNXI_FAKE_SHUTDOWN
/*
 * Support system enter fake shutdown(boot standby), which can be booted up
 * by IR directly. This featch changes the standard reboot route. It also
 * ignores the reboot arg.
 *
 * TODO: This feature should be realized through reboot arg from the userspace
 */
static int sunxi_box_os_enabled;
static int sunxi_box_os_power_off;

static int sunxi_box_power_off(void)
{
	pr_info("sunxi box power off\n");
	sunxi_box_os_power_off = 1;

	/* fake shutdown: restart to boot standby */
	kernel_restart(NULL);

	return true;
}

static int sunxi_box_restart(void)
{
	pr_info("sunxi box restart\n");

	if (sunxi_box_os_power_off)
		rtc_gpreg_write(SUNXI_RTC_GPREG_NUM, SUNXI_BOX_POWEROFF_FLAG);
	else
		rtc_gpreg_write(SUNXI_RTC_GPREG_NUM, SUNXI_BOX_RESTART_FLAG);

	return true;
}

static void sunxi_box_init(void)
{
	script_item_value_type_e item_type;
	script_item_u item;

	item_type = script_get_item("box_start_os", "used", &item);
	if (SCIRPT_ITEM_VALUE_TYPE_INT == item_type)
		sunxi_box_os_enabled = item.val;

	pr_debug("box os state %d\n", sunxi_box_os_enabled);
}
#else
static int sunxi_box_power_off(void)
{
	return false;
}

static int sunxi_box_restart(void)
{
	return false;
}

static void sunxi_box_init(void)
{
}
#endif

#ifdef CONFIG_SUNXI_RTC_REBOOT
static void sunxi_rtc_restart(void *data)
{
	unsigned int rtc_flag = 0;

	if (data == NULL)
		return;

	pr_info("sunxi rtc reboot, arg %s\n", (char *)data);

	if (!strncmp(data, "debug", sizeof("debug"))) {
		rtc_flag = SUNXI_DEBUG_MODE_FLAG;
	} else if (!strncmp(data, "efex", sizeof("efex"))) {
		rtc_flag = SUNXI_EFEX_CMD_FLAG;
	} else if (!strncmp(data, "boot-resignature", sizeof("boot-resignature"))) {
		rtc_flag = SUNXI_BOOT_RESIGNATURE_FLAG;
	} else if (!strncmp(data, "recovery", sizeof("recovery")) \
		|| !strncmp(data, "boot-recovery", sizeof("boot-recovery"))) {
		rtc_flag = SUNXI_BOOT_RECOVERY_FLAG;
	} else if (!strncmp(data, "sysrecovery", sizeof("sysrecovery"))) {
		rtc_flag = SUNXI_SYS_RECOVERY_FLAG;
	} else if (!strncmp(data, "bootloader", sizeof("bootloader"))) {
		rtc_flag = SUNXI_FASTBOOT_FLAG;
	} else if (!strncmp(data, "usb-recovery", sizeof("usb-recovery"))) {
		rtc_flag = SUNXI_USB_RECOVERY_FLAG;
	} else {
		pr_warn("unkown reboot arg %s", (char *)data);
		return;
	}

	rtc_gpreg_write(SUNXI_RTC_GPREG_NUM, rtc_flag);
}
#else
static void sunxi_rtc_restart(void *data)
{
}
#endif

static void sunxi_sys_power_off(void *data)
{
	if (sunxi_box_power_off() == true)
		return;
}

static void sunxi_sys_restart(void *data)
{
	if (sunxi_box_restart() == true)
		return;

	sunxi_rtc_restart(data);
}

static int sunxi_reboot_callback(struct notifier_block *this,
		unsigned long code, void *data)
{
	switch (code) {
	case SYS_POWER_OFF:
		sunxi_sys_power_off(data);
		break;
	case SYS_RESTART:
		sunxi_sys_restart(data);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block sunxi_reboot_notifier = {
	.notifier_call = sunxi_reboot_callback,
};

static int __init sunxi_bootup_extend_init(void)
{
	int ret;

	ret = register_reboot_notifier(&sunxi_reboot_notifier);
	if (ret) {
		pr_err("register reboot notifier error %d\n", ret);
		return -1;
	}

	sunxi_box_init();

	return 0;
}
late_initcall(sunxi_bootup_extend_init);
