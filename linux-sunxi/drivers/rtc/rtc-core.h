/*
 * drivers/rtc/rtc-core.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifdef CONFIG_RTC_INTF_DEV

extern void __init rtc_dev_init(void);
extern void __exit rtc_dev_exit(void);
extern void rtc_dev_prepare(struct rtc_device *rtc);
extern void rtc_dev_add_device(struct rtc_device *rtc);
extern void rtc_dev_del_device(struct rtc_device *rtc);

#else

static inline void rtc_dev_init(void)
{
}

static inline void rtc_dev_exit(void)
{
}

static inline void rtc_dev_prepare(struct rtc_device *rtc)
{
}

static inline void rtc_dev_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_dev_del_device(struct rtc_device *rtc)
{
}

#endif

#ifdef CONFIG_RTC_INTF_PROC

extern void rtc_proc_add_device(struct rtc_device *rtc);
extern void rtc_proc_del_device(struct rtc_device *rtc);

#else

static inline void rtc_proc_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_proc_del_device(struct rtc_device *rtc)
{
}

#endif

#ifdef CONFIG_RTC_INTF_SYSFS

extern void __init rtc_sysfs_init(struct class *);
extern void rtc_sysfs_add_device(struct rtc_device *rtc);
extern void rtc_sysfs_del_device(struct rtc_device *rtc);

#else

static inline void rtc_sysfs_init(struct class *rtc)
{
}

static inline void rtc_sysfs_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_sysfs_del_device(struct rtc_device *rtc)
{
}

#endif
