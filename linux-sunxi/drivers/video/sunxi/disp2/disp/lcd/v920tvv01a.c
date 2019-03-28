/* RGB to Mipi  LCD Driver
 *  LCD type: HP784-4.6BL0292
 *
 * Copyright (c) 2017 Allwinnertech Co., Ltd.
 * Author: huangyixiu <huangyixiu@allwinnertech.com>
 *
 * LCD driver for sunxi platform
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include "v920tvv01a.h"

static void LCD_power_on(u32 sel);
static void LCD_power_off(u32 sel);
static void LCD_bl_open(u32 sel);
static void LCD_bl_close(u32 sel);
static void LCD_panel_init(u32 sel);
static void LCD_panel_exit(u32 sel);


#define SENSOR_NAME "V920T_RGB2MIPI"
#define SENSOR_I2C_SLAVE_ADDRESS (0x1C>>1)

static struct i2c_client *rgb2mipi_i2c_client;
static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static int rgb2mipi_detect(struct i2c_client *client,
		struct i2c_board_info *info)
{
	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
	return 0;
}

static int rgb2mipi_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int rgb2mipi_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t rgb2mipi_read(struct file *filp, char __user *buf, size_t size,
			   loff_t *ppos)
{
	return 1;
}

static const struct file_operations rgb2mipi_fops = {
	.owner = THIS_MODULE,
	.read = rgb2mipi_read,
	.open = rgb2mipi_open,
	.release = rgb2mipi_release,
};

static int lcd_i2c_read_reg_2(struct i2c_client *client, u8 adr,
			u8 *reg, u8 *val)
{
	struct i2c_msg msgs[2] = {{.addr = adr, .flags = 0,
				   .buf = reg, .len = 2},
				  {.addr = adr, .flags = I2C_M_RD,
				   .buf = val, .len = 2} };

	if (i2c_transfer(client->adapter, msgs, 2) != 2) {
		printk(KERN_ERR "error in i2c_read_reg\n");
		return -1;
	}

	return 0;
}

static int lcd_i2c_read_reg(struct i2c_client *client, u8 adr, u16 reg, u16 *val)
{
	u8 ret = 0;
	u8 reg_buf[2];
	u8 val_buf[2];

	reg_buf[0] = reg>>8;
	reg_buf[1] = reg&0xFF;
	ret = lcd_i2c_read_reg_2(client, adr, reg_buf, val_buf);
	*val = (val_buf[0]<<8)|(val_buf[1]);

	return ret;
}

static int lcd_i2c_write_reg_4(struct i2c_client *client, u8 adr, u8 *buf)
{
	struct i2c_msg msgs[1] = { { .addr = adr, .flags = 0,
				   .buf = buf, .len = 4 } };

	if (i2c_transfer(client->adapter, msgs, 1) != 1) {
		printk(KERN_ERR "error in lcd_i2c_write_reg_4\n");
		return -1;
	}

	return 0;
}

static int lcd_i2c_write_reg(struct i2c_client *client, u8 adr, u16 reg, u16 val)
{
	u8 ret = 0;
	u8 buf[4] = {0x00, 0x00, 0x00, 0x00};

	buf[0] = (u8)(reg>>8);
	buf[1] = (u8)(reg&0xFF);
	buf[2] = (u8)(val>>8);
	buf[3] = (u8)(val&0xFF);
	ret = lcd_i2c_write_reg_4(client, adr, buf);

	return ret;
}

static void Delay_iic(__u32 n)
{
	sunxi_lcd_delay_us(n);
}


static void Write_three_Byte_mipi(__u16 reg, __u16 val)
{
	u8 ret = 0;
	ret = lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, reg, val);
}

static int rgb2mipi_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	u16 nData;
	static u8 nInit;

	if (rgb2mipi_i2c_client == NULL)
		rgb2mipi_i2c_client = client;

	if (rgb2mipi_i2c_client != NULL) {
		if (nInit == 0) {
			nInit = 1;
			nData = 0xaaaa;
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0000, &nData);
			printk("lcd v920t id = 0x%x\n", nData);
			Write_three_Byte_mipi(0x0002, 0x0001);
			Delay_iic(30);
			Write_three_Byte_mipi(0x0002, 0x0000);
			/*PLL¿ØÖÆ¼Ä´æÆ÷*/
			Write_three_Byte_mipi(0x0016, 0x003B);
			Write_three_Byte_mipi(0x0018, 0x0603);
			Delay_iic(30);
			Write_three_Byte_mipi(0x0018, 0x0613);
			Write_three_Byte_mipi(0x0006, 0x0258);/*FIFO control register*/

			/*********************D-PHY setting*************************/
			Write_three_Byte_mipi(0x0140, 0x0000);/*D-PHY Clock lane enable*/
			Write_three_Byte_mipi(0x0142, 0x0000);
			Write_three_Byte_mipi(0x0144, 0x0000);/*D-PHY Data lane0 enable*/
			Write_three_Byte_mipi(0x0146, 0x0000);
			Write_three_Byte_mipi(0x0148, 0x0000);/*D-PHY Data lane1 enable*/
			Write_three_Byte_mipi(0x014A, 0x0000);
			Write_three_Byte_mipi(0x014C, 0x0000);/*D-PHY Data lane2 enable*/
			Write_three_Byte_mipi(0x014E, 0x0000);
			Write_three_Byte_mipi(0x0150, 0x0000);/*D-PHY Data lane3 enable*/
			Write_three_Byte_mipi(0x0152, 0x0000);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0100, 0x0002);/*D-PHY Clock lane control*/
			Write_three_Byte_mipi(0x0102, 0x0000);
			Write_three_Byte_mipi(0x0104, 0x0002);/*D-PHY Data lane0 control*/
			Write_three_Byte_mipi(0x0106, 0x0000);
			Write_three_Byte_mipi(0x0108, 0x0002);/*D-PHY Data lane1 control*/
			Write_three_Byte_mipi(0x010A, 0x0000);
			Write_three_Byte_mipi(0x010C, 0x0002);/*D-PHY Data lane2 control*/
			Write_three_Byte_mipi(0x010E, 0x0000);
			Write_three_Byte_mipi(0x0110, 0x0002);/*D-PHY Data lane3 control*/
			Write_three_Byte_mipi(0x0112, 0x0000);

			Delay_iic(300);

			/*DSI-TX PPI Control*/
			Write_three_Byte_mipi(0x0210, 0x0BB8);
			Write_three_Byte_mipi(0x0212, 0x0000);
			Write_three_Byte_mipi(0x0214, 0x0002);
			Write_three_Byte_mipi(0x0216, 0x0000);
			Write_three_Byte_mipi(0x0218, 0x1402);
			Write_three_Byte_mipi(0x021A, 0x0000);
			Write_three_Byte_mipi(0x021C, 0x0000);
			Write_three_Byte_mipi(0x021E, 0x0000);
			Write_three_Byte_mipi(0x0220, 0x0002);
			Write_three_Byte_mipi(0x0222, 0x0000);
			Write_three_Byte_mipi(0x0224, 0x3A98);

			Delay_iic(300);
			Write_three_Byte_mipi(0x0226, 0x0000);
			Write_three_Byte_mipi(0x0228, 0x0006);
			Write_three_Byte_mipi(0x022A, 0x0000);
			Write_three_Byte_mipi(0x022C, 0x0000);
			Write_three_Byte_mipi(0x022E, 0x0000);
			Write_three_Byte_mipi(0x0230, 0x0005);
			Write_three_Byte_mipi(0x0232, 0x0000);
			Delay_iic(300);

			Write_three_Byte_mipi(0x0234, 0x001F);
			Write_three_Byte_mipi(0x0236, 0x0000);
			Write_three_Byte_mipi(0x0238, 0x0001);
			Write_three_Byte_mipi(0x023A, 0x0000);
			Write_three_Byte_mipi(0x023C, 0x0003);
			Write_three_Byte_mipi(0x023E, 0x0002);
			Write_three_Byte_mipi(0x0204, 0x0001);
			Write_three_Byte_mipi(0x0206, 0x0000);

			Delay_iic(300);

			Write_three_Byte_mipi(0x0620, 0x0001);
			Write_three_Byte_mipi(0x0622, 0x001E);
			Write_three_Byte_mipi(0x0624, 0x001C);
			Write_three_Byte_mipi(0x0626, 0x0140);
			Write_three_Byte_mipi(0x0628, 0x019D);
			Write_three_Byte_mipi(0x062A, 0x012C);
			Write_three_Byte_mipi(0x062C, 0x0A00);
			Write_three_Byte_mipi(0x0518, 0x0001);
			Write_three_Byte_mipi(0x051A, 0x0000);

			/*init 9.2 code  */
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0xAACD);
			Write_three_Byte_mipi(0x0600, 0x0001);

			Delay_iic(300);

			Write_three_Byte_mipi(0x0008, 0x0001);
			Write_three_Byte_mipi(0x0050, 0x0039);
			Write_three_Byte_mipi(0x0022, 0x0017);
			Write_three_Byte_mipi(0x00E0, 0x8000);
			Write_three_Byte_mipi(0x00E8, 0x1352);
			Write_three_Byte_mipi(0x00E8, 0x0013);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x1213);
			Write_three_Byte_mipi(0x00E8, 0x0607);
			Write_three_Byte_mipi(0x00E8, 0x1305);
			Write_three_Byte_mipi(0x00E8, 0x0413);
			Write_three_Byte_mipi(0x00E8, 0x1302);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x1303);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x0013);
			Delay_iic(300);
			Write_three_Byte_mipi(0x00E8, 0x0000);
			Write_three_Byte_mipi(0x00E0, 0xE000);
			Write_three_Byte_mipi(0x00E0, 0x2000);
			Write_three_Byte_mipi(0x00E0, 0x0000);

			Write_three_Byte_mipi(0x0008, 0x0001);
			Write_three_Byte_mipi(0x0050, 0x0039);
			Write_three_Byte_mipi(0x0022, 0x0017);
			Write_three_Byte_mipi(0x00E0, 0x8000);
			Write_three_Byte_mipi(0x00E8, 0x1359);
			Write_three_Byte_mipi(0x00E8, 0x0013);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x1213);
			Write_three_Byte_mipi(0x00E8, 0x0607);
			Write_three_Byte_mipi(0x00E8, 0x1305);
			Write_three_Byte_mipi(0x00E8, 0x0413);
			Write_three_Byte_mipi(0x00E8, 0x1302);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x1303);
			Write_three_Byte_mipi(0x00E8, 0x1313);
			Write_three_Byte_mipi(0x00E8, 0x0013);
			Delay_iic(300);
			Write_three_Byte_mipi(0x00E8, 0x0000);
			Write_three_Byte_mipi(0x00E0, 0xE000);
			Write_three_Byte_mipi(0x00E0, 0x2000);
			Write_three_Byte_mipi(0x00E0, 0x0000);

			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x0067);
			Write_three_Byte_mipi(0x0600, 0x0001);

			Delay_iic(300);

			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x886C);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x0456);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0008, 0x0001);
			Write_three_Byte_mipi(0x0050, 0x0039);
			Write_three_Byte_mipi(0x0022, 0x000D);
			Write_three_Byte_mipi(0x00E0, 0x8000);
			Write_three_Byte_mipi(0x00E8, 0x0055);
			Write_three_Byte_mipi(0x00E8, 0x3200);
			Write_three_Byte_mipi(0x00E8, 0x0032);
			Write_three_Byte_mipi(0x00E8, 0x3200);
			Write_three_Byte_mipi(0x00E8, 0x0032);
			Write_three_Byte_mipi(0x00E8, 0x0F00);
			Write_three_Byte_mipi(0x00E8, 0x000F);
			Delay_iic(300);
			Write_three_Byte_mipi(0x00E8, 0x0000);
			Write_three_Byte_mipi(0x00E0, 0xE000);
			Write_three_Byte_mipi(0x00E0, 0x2000);
			Write_three_Byte_mipi(0x00E0, 0x0000);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x496B);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0xFF18);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x3F19);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0xCC1A);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x0232);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x2D3A);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x006D);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x2A4E);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x2D4F);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0008, 0x0001);
			Write_three_Byte_mipi(0x0050, 0x0039);
			Write_three_Byte_mipi(0x0022, 0x000E);
			Write_three_Byte_mipi(0x00E0, 0x8000);
			Write_three_Byte_mipi(0x00E8, 0x3F53);
			Write_three_Byte_mipi(0x00E8, 0x0C0A);
			Write_three_Byte_mipi(0x00E8, 0x120B);
			Write_three_Byte_mipi(0x00E8, 0x0D13);
			Write_three_Byte_mipi(0x00E8, 0x0803);
			Write_three_Byte_mipi(0x00E8, 0x0506);
			Write_three_Byte_mipi(0x00E8, 0x140B);
			Delay_iic(300);
			Write_three_Byte_mipi(0x00E8, 0x0000);
			Write_three_Byte_mipi(0x00E0, 0xE000);
			Write_three_Byte_mipi(0x00E0, 0x2000);
			Write_three_Byte_mipi(0x00E0, 0x0000);

			Write_three_Byte_mipi(0x0008, 0x0001);
			Write_three_Byte_mipi(0x0050, 0x0039);
			Write_three_Byte_mipi(0x0022, 0x000E);
			Write_three_Byte_mipi(0x00E0, 0x8000);
			Write_three_Byte_mipi(0x00E8, 0x3F54);
			Write_three_Byte_mipi(0x00E8, 0x0C0A);
			Write_three_Byte_mipi(0x00E8, 0x120B);
			Write_three_Byte_mipi(0x00E8, 0x0D13);
			Write_three_Byte_mipi(0x00E8, 0x0803);
			Write_three_Byte_mipi(0x00E8, 0x0506);
			Write_three_Byte_mipi(0x00E8, 0x140B);
			Delay_iic(300);
			Write_three_Byte_mipi(0x00E8, 0x0000);
			Write_three_Byte_mipi(0x00E0, 0xE000);
			Write_three_Byte_mipi(0x00E0, 0x2000);
			Write_three_Byte_mipi(0x00E0, 0x0000);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x8051);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x0D0E);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0xF073);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x9174);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x0375);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0xE371);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x483C);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x425E);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x1435);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x177A);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x074A);
			Write_three_Byte_mipi(0x0600, 0x0001);
			Delay_iic(300);
			Write_three_Byte_mipi(0x0602, 0x4029);
			Write_three_Byte_mipi(0x0604, 0x0002);
			Write_three_Byte_mipi(0x0610, 0x004D);
			Write_three_Byte_mipi(0x0600, 0x0001);

			Delay_iic(300);

			/*ÑÓÊ±50ms*/
			/*REM Set to HS mode*/
			Delay_iic(300);
			Write_three_Byte_mipi(0x0500, 0x0086);
			Write_three_Byte_mipi(0x0502, 0xA300);
			Write_three_Byte_mipi(0x0500, 0x8000);
			Write_three_Byte_mipi(0x0502, 0xC300);
			/*HOST RGB INPUT START*/
			Write_three_Byte_mipi(0x0008, 0x0057);
			Write_three_Byte_mipi(0x0050, 0x000E);
			Write_three_Byte_mipi(0x0032, 0x0000);
			Write_three_Byte_mipi(0x0004, 0x0044);
			Delay_iic(300);

		  printk("lcd v920t init mipi OK \n");
		}
	}

	if (rgb2mipi_i2c_client == NULL)
		printk(KERN_INFO "==%s== failed\r\n", __func__);

	return 0;
}

static int rgb2mipi_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id t_rgb2mipi_id[] = {
	{SENSOR_NAME, 2},
	{}
};

/*MODULE_DEVICE_TABLE(i2c, t_rgb2mipi_id);*/

static struct i2c_driver rgb2mipi_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = rgb2mipi_probe,
	.remove = rgb2mipi_remove,
	.id_table = t_rgb2mipi_id,
	.detect = rgb2mipi_detect,
	.address_list = normal_i2c,
};

static int rgb2mipi_init(void)
{
	int ret = -1;

	ret = i2c_add_driver(&rgb2mipi_driver);
	if (ret < 0) {
		printk(KERN_INFO "add i2c driver failed\n");
		return -1;
	}

	return ret;
}

static void rgb2mipi_exit(void)
{
	i2c_del_driver(&rgb2mipi_driver);
}

static void LCD_cfg_panel_info(panel_extend_para *info)
{
	u32 i = 0, j = 0;
	u32 items;
	u32 value = 0;
	u32 num = 0;
	u8 lcd_gamma_tbl[][2] = {
	    /*{input value, corrected value}*/
		{0, 0},
		{15, 15},
		{30, 30},
		{45, 45},
		{60, 60},
		{75, 75},
		{90, 90},
		{105, 105},
		{120, 120},
		{135, 135},
		{150, 150},
		{165, 165},
		{180, 180},
		{195, 195},
		{210, 210},
		{225, 225},
		{240, 240},
		{255, 255},
	};

	u32 lcd_cmap_tbl[2][3][4] = {
	{
		{LCD_CMAP_G0, LCD_CMAP_B1, LCD_CMAP_G2, LCD_CMAP_B3},
		{LCD_CMAP_B0, LCD_CMAP_R1, LCD_CMAP_B2, LCD_CMAP_R3},
		{LCD_CMAP_R0, LCD_CMAP_G1, LCD_CMAP_R2, LCD_CMAP_G3},
		},
		{
		{LCD_CMAP_B3, LCD_CMAP_G2, LCD_CMAP_B1, LCD_CMAP_G0},
		{LCD_CMAP_R3, LCD_CMAP_B2, LCD_CMAP_R1, LCD_CMAP_B0},
		{LCD_CMAP_G3, LCD_CMAP_R2, LCD_CMAP_G1, LCD_CMAP_R0},
		},
	};

	items = sizeof(lcd_gamma_tbl)/2;
	for (i = 0; i < items-1; i++) {
		num = lcd_gamma_tbl[i+1][0] - lcd_gamma_tbl[i][0];

		for (j = 0; j < num; j++) {
			value = lcd_gamma_tbl[i][1] + ((lcd_gamma_tbl[i + 1][1]
				- lcd_gamma_tbl[i][1]) * j)/num;
			info->lcd_gamma_tbl[lcd_gamma_tbl[i][0] + j]
				= (value<<16) + (value<<8) + value;
		}
	}
	info->lcd_gamma_tbl[255] = (lcd_gamma_tbl[items-1][1]<<16)
		+ (lcd_gamma_tbl[items-1][1]<<8) + lcd_gamma_tbl[items-1][1];
	memcpy(info->lcd_cmap_tbl, lcd_cmap_tbl, sizeof(lcd_cmap_tbl));
}

static s32 LCD_open_flow(u32 sel)
{
	LCD_OPEN_FUNC(sel, LCD_power_on, 0);
	LCD_OPEN_FUNC(sel, sunxi_lcd_tcon_enable, 0);
	LCD_OPEN_FUNC(sel, LCD_panel_init, 0);
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);
	return 0;
}

static s32 LCD_close_flow(u32 sel)
{
	LCD_CLOSE_FUNC(sel, LCD_bl_close, 0);
	LCD_CLOSE_FUNC(sel, sunxi_lcd_tcon_disable, 0);
	LCD_CLOSE_FUNC(sel, LCD_panel_exit, 100);
	LCD_CLOSE_FUNC(sel, LCD_power_off, 100);

	return 0;
}

static void LCD_power_on(u32 sel)
{
	printk("GW LCD power on \n");
	sunxi_lcd_power_enable(sel, 0);
	sunxi_lcd_delay_ms(1);
	sunxi_lcd_gpio_set_value(sel, 0, 0);
	sunxi_lcd_delay_ms(1);
	sunxi_lcd_gpio_set_value(sel, 0, 1);
	sunxi_lcd_pin_cfg(sel, 1);
}

static void LCD_power_off(u32 sel)
{
	sunxi_lcd_pin_cfg(sel, 0);
	sunxi_lcd_gpio_set_value(sel, 0, 0);
	sunxi_lcd_power_disable(sel, 0);
}

static void LCD_bl_open(u32 sel)
{
	printk("GW LCD bl open\n");
	sunxi_lcd_pwm_enable(sel);
	sunxi_lcd_backlight_enable(sel);
}

static void LCD_bl_close(u32 sel)
{
	sunxi_lcd_backlight_disable(sel);
	sunxi_lcd_pwm_disable(sel);
}

static void LCD_panel_init(u32 sel)
{
	printk("rgb2mipi_init \n");
	rgb2mipi_init();

	return;
}

static void LCD_panel_exit(u32 sel)
{
	rgb2mipi_exit();

	return ;
}

static s32 LCD_user_defined_func(u32 sel,
		u32 para1, u32 para2, u32 para3)
{
	return 0;
}

__lcd_panel_t v920tvv01a_panel = {
	/* panel driver name, must mach the name of
	    lcd_drv_name in sys_config.fex */
	.name = "v920tvv01a",
	.func = {
		.cfg_panel_info = LCD_cfg_panel_info,
		.cfg_open_flow = LCD_open_flow,
		.cfg_close_flow = LCD_close_flow,
		.lcd_user_defined_func = LCD_user_defined_func,
	},
};
