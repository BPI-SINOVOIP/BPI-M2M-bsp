/*
 * arch/mips/kernel/i8253.c
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
 * i8253.c  8253/PIT functions
 *
 */
#include <linux/clockchips.h>
#include <linux/i8253.h>
#include <linux/export.h>
#include <linux/smp.h>
#include <linux/irq.h>

#include <asm/time.h>

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	i8253_clockevent.event_handler(&i8253_clockevent);

	return IRQ_HANDLED;
}

static struct irqaction irq0  = {
	.handler = timer_interrupt,
	.flags = IRQF_NOBALANCING | IRQF_TIMER,
	.name = "timer"
};

void __init setup_pit_timer(void)
{
	clockevent_i8253_init(true);
	setup_irq(0, &irq0);
}

static int __init init_pit_clocksource(void)
{
	if (num_possible_cpus() > 1) /* PIT does not scale! */
		return 0;

	return clocksource_i8253_init();
}
arch_initcall(init_pit_clocksource);
