/*
 * include/linux/mfd/axp-mfd-259.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef __LINUX_AXP_MFD_259_H_
#define __LINUX_AXP_MFD_259_H_

/* Unified sub device IDs for AXP */
/* LDO0 For RTCLDO ,LDO1-3 for ALDO,LDO*/

/*For AXP259*/
#define AXP259                     (259)
#define AXP259_STATUS              (0x00)
#define AXP259_MODE_CHGSTATUS      (0x01)
#define AXP259_IC_TYPE             (0x03)

#define AXP259_BUFFER1             (0x05)
#define AXP259_BUFFER2             (0x06)
#define AXP259_BUFFER3             (0x07)
#define AXP259_BUFFER4             (0x08)
#define AXP259_BUFFER5             (0x09)
#define AXP259_BUFFER6             (0x0A)
#define AXP259_BUFFER7             (0x0B)
#define AXP259_BUFFER8             (0x0C)
#define AXP259_BUFFER9             (0x0D)
#define AXP259_BUFFERA             (0x0E)
#define AXP259_BUFFERB             (0x0F)

#define AXP259_IPS_SET			   (0x000)
#define AXP259_VOFF_SET            (0x000)

#define AXP259_GPIO1_CTL           (0x18)
#define AXP259_GPIO2_CTL           (0x19)
#define AXP259_GPIO01_SIGNAL       (0x1A)

#define AXP259_CHARGE1             (0x20)
#define AXP259_CHARGE2             (0x21)
#define AXP259_CHARGE3             (0x22)
#define AXP259_CHARGE4             (0x23)

#define AXP259_ADC_EN              (0x24)
#define AXP259_TS_PIN_CONTROL      (0x25)
#define AXP259_POK_SET             (0x27)
#define AXP259_OFF_CTL             (0x28)
#define AXP259_INTEN1              (0x40)
#define AXP259_INTEN2              (0x41)
#define AXP259_INTEN3              (0x42)
#define AXP259_INTEN4              (0x43)
#define AXP259_INTEN5              (0x44)
#define AXP259_INTSTS1             (0x48)
#define AXP259_INTSTS2             (0x49)
#define AXP259_INTSTS3             (0x4A)
#define AXP259_INTSTS4             (0x4B)
#define AXP259_INTSTS5             (0x4C)
#define AXP259_WARNING_LEVEL       (0xE6)
#define AXP259_ADDR_EXTENSION      (0xFF)

/* bit definitions for AXP events ,irq event */
/*  AXP259  */
#define AXP259_IRQ_ICTEMOV       (1 << 0)
#define AXP259_IRQ_PMOSEN        (1 << 1)
#define AXP259_IRQ_BUCKLO        (1 << 2)
#define AXP259_IRQ_BUCKHI        (1 << 3)
#define AXP259_IRQ_ACRE          (1 << 4)
#define AXP259_IRQ_ACIN          (1 << 5)
#define AXP259_IRQ_ACOV          (1 << 6)
#define AXP259_IRQ_VACIN         (1 << 7)
#define AXP259_IRQ_LOWN2         (1 << 8)
#define AXP259_IRQ_LOWN1         (1 << 9)
#define AXP259_IRQ_CHAOV         (1 << 10)
#define AXP259_IRQ_CHAST         (1 << 11)
#define AXP259_IRQ_BATSAFE_QUIT  (1 << 12)
#define AXP259_IRQ_BATSAFE_ENTER (1 << 13)
#define AXP259_IRQ_BATRE         (1 << 14)
#define AXP259_IRQ_BATIN         (1 << 15)
#define AXP259_IRQ_QBWUT         (1 << 16)
#define AXP259_IRQ_BWUT          (1 << 17)
#define AXP259_IRQ_QBWOT         (1 << 18)
#define AXP259_IRQ_BWOT          (1 << 19)
#define AXP259_IRQ_QBCUT         (1 << 20)
#define AXP259_IRQ_BCUT          (1 << 21)
#define AXP259_IRQ_QBCOT         (1 << 22)
#define AXP259_IRQ_BCOT          (1 << 23)
#define AXP259_IRQ_GPIO0         (1 << 24)
#define AXP259_IRQ_BATCHG        (1 << 25)
#define AXP259_IRQ_POKOFF        (1 << 26)
#define AXP259_IRQ_POKLO         (1 << 27)
#define AXP259_IRQ_POKSH         (1 << 28)
#define AXP259_IRQ_PEKFE         (1 << 29)
#define AXP259_IRQ_PEKRE         (1 << 30)
#define AXP259_IRQ_BUCKOV_6V6    ((uint64_t)1 << 32)

/* Status Query Interface */
/*  AXP259  */
#define AXP259_STATUS_SOURCE	(1 <<  0)
#define AXP259_STATUS_ACUSBSH	(1 <<  1)
#define AXP259_STATUS_BATCURDIR	(1 <<  2)
#define AXP259_STATUS_USBLAVHO	(1 <<  3)
#define AXP259_STATUS_USBVA	(1 <<  4)
#define AXP259_STATUS_USBEN	(1 <<  5)
#define AXP259_STATUS_ACVA	(1 <<  6)

#define AXP259_STATUS_INCHAR	(1 << 14)

#endif /* __LINUX_AXP_MFD_259_H_ */
