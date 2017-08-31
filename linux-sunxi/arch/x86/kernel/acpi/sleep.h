/*
 * arch/x86/kernel/acpi/sleep.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *	Variables and functions used by the code in sleep.c
 */

#include <asm/trampoline.h>
#include <linux/linkage.h>

extern unsigned long saved_video_mode;
extern long saved_magic;

extern int wakeup_pmode_return;

extern u8 wake_sleep_flags;
extern asmlinkage void acpi_enter_s3(void);

extern unsigned long acpi_copy_wakeup_routine(unsigned long);
extern void wakeup_long64(void);

extern void do_suspend_lowlevel(void);
