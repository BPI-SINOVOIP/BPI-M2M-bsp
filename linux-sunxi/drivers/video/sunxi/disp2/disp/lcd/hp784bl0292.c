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
#include "hp784bl0292.h"

static void LCD_power_on(u32 sel);
static void LCD_power_off(u32 sel);
static void LCD_bl_open(u32 sel);
static void LCD_bl_close(u32 sel);
static void LCD_panel_init(u32 sel);
static void LCD_panel_exit(u32 sel);


#define SENSOR_NAME "RGB2MIPI"
#define SENSOR_I2C_SLAVE_ADDRESS (0x1C>>1)

static struct i2c_client *rgb2mipi_i2c_client;
static const unsigned short normal_i2c[2] = {
	SENSOR_I2C_SLAVE_ADDRESS, I2C_CLIENT_END };

static int rgb2mipi_read8(struct i2c_client *client, uint8_t reg, uint8_t *data)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, reg, 1, data);

	return ret;
}

static int rgb2mipi_write8(struct i2c_client *client, uint8_t reg, uint8_t val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, val);

	return ret;
}

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

static int lcd_i2c_write_reg_2(struct i2c_client *client, u8 adr,
			u8 *reg, u8 *val)
{
	struct i2c_msg msgs[2] = {{.addr = adr, .flags = 0,
				   .buf = reg, .len = 2},
				  {.addr = adr, .flags = I2C_M_NOSTART,
				   .buf = val, .len = 2} };

	if (i2c_transfer(client->adapter, msgs, 2) != 2) {
		printk(KERN_ERR "error in i2c_read_reg\n");
		return -1;
	}

	return 0;
}

static int lcd_i2c_write_reg_4(struct i2c_client *client, u8 adr, u8 *buf)
{
	struct i2c_msg msgs[1] = { { .addr = adr, .flags = 0,
				   .buf = buf, .len = 4 } };

	if (i2c_transfer(client->adapter, msgs, 1) != 1) {
		printk(KERN_ERR "error in i2c_read_reg\n");
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

static int rgb2mipi_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	u16 nData;
	static u8 nInit;
	__u32 value;
	__u32 read_value;

	if (rgb2mipi_i2c_client == NULL)
		rgb2mipi_i2c_client = client;

	if (rgb2mipi_i2c_client != NULL) {
		if (nInit == 0) {
			nInit = 1;
			nData = 0xaaaa;
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0000, &nData);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010,
				(value & (~(0x3 << 1))));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010, (u16 *)&value);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014,
				value | (0x3 << 1));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0000, (u16 *)&value);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0002, 0x0001);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0002, 0x0000);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0016, 0x003B);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0018, 0x0603);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0018, 0x0613);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0006, 0x00C8);
			/* D-PHY Clock lane enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0140, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0142, 0x0000);
			/* D-PHY Data lane0 enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0144, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0146, 0x0000);
			/* D-PHY Data lane1 enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0148, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014A, 0x0000);
			/* D-PHY Data lane2 enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014C, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014E, 0x0000);
			/* D-PHY Data lane3 enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0150, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0152, 0x0000);
			/* D-PHY Clock lane control */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0100, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0102, 0x0000);
			/* D-PHY Data lane0 control */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0104, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0106, 0x0000);
			/* D-PHY Data lane1 control */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0108, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010A, 0x0000);
			/* D-PHY Data lane2 control */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010C, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010E, 0x0000);
			/* D-PHY Data lane3 control */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0110, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0112, 0x0000);
			/* LINEINITCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0210, 0x1644);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0212, 0x0000);
			/* LPTXTIMECNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0214, 0x0004);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0216, 0x0000);
			/* TCLK_HEADERCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0218, 0x2002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021A, 0x0000);
			/* TCLK_TRAILCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021C, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021E, 0x0000);
			/* THS_HEADERCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0220, 0x0602);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0222, 0x0000);
			/* TWAKEUPCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0224, 0x5208);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0226, 0x0000);
			/* THS_TRAILCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x022C, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x022E, 0x0000);
			/* HSTXVREGCNT */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0230, 0x0005);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0232, 0x0000);
			/* HSTXVREGEN enable */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0234, 0x001F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0236, 0x0000);
			/* DSI clock Enable/Disable during LP */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0238, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023A, 0x0000);
			/* BTACNTRL1 */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023C, 0x0002);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023E, 0x0002);
			/* STARTCNTRL */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0204, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0206, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0620, 0x0001);
			/* V Control Register1 */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0622, 0x000C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0624, 0x000F);
			/* V Control Register3 */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0626, 0x0500);
			/* H Control Register1 */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0628, 0x02B2);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x062A, 0x0339);
			/* H Control Register3 */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x062C, 0x04B0);
			/* DSI Start */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0518, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x051A, 0x0000);
		    sunxi_lcd_delay_ms(5);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr,
				0x00E, (value | 0xff00));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E, (u16 *)&value);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010,
				((value | 0xff00) & (~(0x3 << 1))));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0010, (u16 *)&value);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014,
				value & (~(0x3 << 1)));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			sunxi_lcd_delay_ms(10);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014,
				value | (0x1 << 1));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0xAACD);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(100);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x143A);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
		    sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0032);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0236);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x8267);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x2069);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x016D);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0068);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x0039);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0022, 0x0014);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1953);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1517);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1212);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1312);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1515);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0C10);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0A0A);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0B0C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x090C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0607);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0xE000);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x2000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x0000);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x0039);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0022, 0x0014);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1954);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1416);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1111);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1311);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1515);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0B10);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0909);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0B0B);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x090C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0607);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0xE000);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x2000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x0000);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x1129);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0C2A);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x816C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x0039);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0022, 0x0009);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0055);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0xE000);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x2000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x0000);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x0039);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0022, 0x0014);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x1256);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x121F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x001F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0xE000);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x2000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x0000);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0001);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x0039);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0022, 0x0005);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0057);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x000F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E8, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0xE000);
			sunxi_lcd_delay_ms(1);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x2000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x00E0, 0x0000);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0030);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0833);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x2735);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x3D4F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x354E);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x3541);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x3073);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x1074);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x4076);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0077);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x7128);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x807C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x042E);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x804C);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x1647);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x6C48);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0xC050);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x6E78);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x312D);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0463);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x8461);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0xAC65);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x3339);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0049);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(100);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0912);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x1F3D);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0xEF3E);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x1E3F);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0427);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x021B);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x061B);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(1);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1015);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x004D);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
			sunxi_lcd_delay_ms(100);
			/* DSI lane setting, DSI mode=HS */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0500, 0x0086);
			/* bit set */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0502, 0xA300);
			/* Switch to DSI mode */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0500, 0x8000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0502, 0xC300);
			/* DSI-TX Format setting */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, 0x0037);
			/* DSI-TX Pixel stream packet Data Type setting */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, 0x003E);
			/* HSYNC Polarity */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0032, 0x0000);
			/* Configuration Control Register */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0004, 0x0044);
			/* bit15:8=Packet Type:Short Packet, bit5:0=Data
			 * Type DCS Short Write (no parameter) */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0602, 0x1005);
			/* bit7:6=Word Count(Lower Byte), bit15:8=Word
			 * Count(Upper Byte) */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0604, 0x0000);
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0610, 0x0029);
			/* Packet Transfer */
		    lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0600, 0x0001);
		    sunxi_lcd_delay_ms(100);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			lcd_i2c_write_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014,
				value | (0x1 << 2));
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0014, (u16 *)&value);
			sunxi_lcd_delay_ms(10);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0002, (u16 *)&read_value);
			/* PLL Control Register 0 (PLL_PRD,PLL_FBD) */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0016, (u16 *)&read_value);
			/* PLL_FRS,PLL_LBWS, PLL oscillation enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0018, (u16 *)&read_value);
		    sunxi_lcd_delay_ms(1);
			/* PLL_FRS,PLL_LBWS, PLL clock out enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0018, (u16 *)&read_value);
			/* FIFO Control Register */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0006, (u16 *)&read_value);
			/* D-PHY Clock lane enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0140, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0142, (u16 *)&read_value);
			/* D-PHY Data lane0 enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0144, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0146, (u16 *)&read_value);
			/* D-PHY Data lane1 enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0148, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014A, (u16 *)&read_value);
			/* D-PHY Data lane2 enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x014E, (u16 *)&read_value);
			/* D-PHY Data lane3 enable */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0150, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0152, (u16 *)&read_value);
			/* D-PHY Clock lane control */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0100, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0102, (u16 *)&read_value);
			/* D-PHY Data lane0 control */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0104, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0106, (u16 *)&read_value);
			/* D-PHY Data lane1 control */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0108, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010A, (u16 *)&read_value);
			/* D-PHY Data lane2 control */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x010E, (u16 *)&read_value);
			/* D-PHY Data lane3 control */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0110, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0112, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0210, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0212, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0214, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0216, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0218, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021A, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x021E, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0220, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0222, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0224, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0226, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0228, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x022A, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x022C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x022E, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0230, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0232, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0234, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0236, (u16 *)&read_value);
			/* DSI clock Enable/Disable during LP */
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0238, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023A, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x023E, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0204, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0206, (u16 *)&read_value);
			/* Sync Pulse/Sync Event mode setting */
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0620, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0622, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0624, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0626, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0628, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x062A, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x062C, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0518, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x051A, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0500, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0502, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0500, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0502, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0008, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0050, (u16 *)&read_value);
		    lcd_i2c_read_reg(rgb2mipi_i2c_client,
				rgb2mipi_i2c_client->addr, 0x0032, (u16 *)&read_value);
			lcd_i2c_read_reg(rgb2mipi_i2c_client,
			rgb2mipi_i2c_client->addr, 0x0004, (u16 *)&read_value);
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

static const struct i2c_device_id rgb2mipi_id[] = {
	{SENSOR_NAME, 1},
	{}
};

MODULE_DEVICE_TABLE(i2c, rgb2mipi_id);

static struct i2c_driver rgb2mipi_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		   .name = SENSOR_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = rgb2mipi_probe,
	.remove = rgb2mipi_remove,
	.id_table = rgb2mipi_id,
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
	LCD_OPEN_FUNC(sel, LCD_power_on, 20);
	LCD_OPEN_FUNC(sel, sunxi_lcd_tcon_enable, 100);
	LCD_OPEN_FUNC(sel, LCD_bl_open, 50);
	LCD_OPEN_FUNC(sel, LCD_panel_init, 10);

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
	__u32 nData = 0;

	sunxi_lcd_power_enable(sel, 0);
	nData = (get_wvalue(0xF1c20834)&(0x01<<3))>>3;
	sunxi_lcd_delay_ms(1);
	sunxi_lcd_gpio_set_value(sel, 0, 0);
	nData = (get_wvalue(0xF1c208E8)&(0x01<<9))>>9;
	sunxi_lcd_delay_ms(1);
	sunxi_lcd_gpio_set_value(sel, 0, 1);
	nData = (get_wvalue(0xF1c208E8)&(0x01<<9))>>9;
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

__lcd_panel_t hp784bl0292_panel = {
	/* panel driver name, must mach the name of
	    lcd_drv_name in sys_config.fex */
	.name = "hp784bl0292",
	.func = {
		.cfg_panel_info = LCD_cfg_panel_info,
		.cfg_open_flow = LCD_open_flow,
		.cfg_close_flow = LCD_close_flow,
		.lcd_user_defined_func = LCD_user_defined_func,
	},
};
