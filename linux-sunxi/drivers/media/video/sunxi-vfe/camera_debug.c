
#include "camera_debug.h"
#include "utility/vfe_io.h"
#include <mach/platform.h>


unsigned int mipi_dphy_dbg;
unsigned int mipi_csi2_dbg;

#if defined CONFIG_ARCH_SUN8IW1P1 || defined CONFIG_ARCH_SUN8IW8P1
void mipi_dphy_reg(struct vfe_dev *dev)
{
	void __iomem *dphy_reg = dev->regs.csi_regs + 0x2000;
	u32 i = 0, j = 0;
	u32 rval;
	u32 lanes, dphy_en, rxclk_en, mipi_bps;
	char *rxdata_en = NULL, *clk_mode = NULL;
	char dm[4][10] = {"\0", "\0", "\0", "\0"}, data_mode[40] = "\0";

	u32 clk_st0 = 0, clk_st1 = 0, clk_diff = 0, clk_err = 0;
	u32 data_st0 = 0, data_st1 = 0, data_diff = 0, data_err = 0;

	rval = vfe_reg_readl(dphy_reg);
	lanes = ((rval & 0x00000030) >> 4) + 1;
	dphy_en = rval & 0x00000001;

	rval = vfe_reg_readl(dphy_reg + 0x8);
	rxclk_en = (rval & 0x01000000) >> 24;

	switch ((rval & 0x00f00000) >> 20) {
	case 0x1:
		rxdata_en = "d0 enable";
		break;
	case 0x3:
		rxdata_en = "d0~d1 enable";
		break;
	case 0x7:
		rxdata_en = "d0~d2 enable";
		break;
	case 0xf:
		rxdata_en = "d0~d3 enable";
		break;
	default:
		rxdata_en = "error";
		break;
	}

	vfe_reg_writel(dphy_reg + 0x30, 0xffffff02);

	do {
		rval = vfe_reg_readl(dphy_reg + 0x40);
	} while (!rval);
	mipi_bps = 150*8000/(rval & 0x0000ffff);

	for (i = 0; i < 50; i++) {
		rval = vfe_reg_readl(dphy_reg + 0xec);
		clk_st0 = (rval & 0x000f0000) >> 16;
		usleep_range(1000, 1200);
		rval = vfe_reg_readl(dphy_reg + 0xec);
		clk_st1 = (rval & 0x000f0000) >> 16;
		if (((0x5 == clk_st0) || (0x3 == clk_st0) ||
		    (0x0 == clk_st0) || (0x1 == clk_st0)) &&
		    ((0x5 == clk_st1) || (0x3 == clk_st1) ||
		    (0x0 == clk_st1) || (0x1 == clk_st1))) {
			if (clk_st0 != clk_st1) {
				clk_diff = 1;
				break;
			}
		} else {
			clk_err = 1;
		}
	}

	if (clk_err)
		clk_mode = "error";
	else if (clk_diff)
		clk_mode = "uncontinuous";
	else {
		if (clk_st0 == 0x5)
			clk_mode = "continuous";
		if (clk_st0 == 0x0)
			clk_mode = "LP00";
		if (clk_st0 == 0x3)
			clk_mode = "LP11";
		if (clk_st0 == 0x1)
			clk_mode = "LP01";
	}

	for (i = 0; i < lanes; i++) {
		for (j = 0; j < 50; j++) {
			rval = vfe_reg_readl(dphy_reg + 0xec);
			data_st0 = rval & (0xf << (i * 4));
			usleep_range(1000, 1200);
			rval = vfe_reg_readl(dphy_reg + 0xec);
			data_st1 = rval & (0xf << (i * 4));
			if (((0x5 == data_st0) || (0x3 == data_st0) ||
			    (0x0 == data_st0) || (0x1 == data_st0)) &&
			    ((0x5 == data_st1) || (0x3 == data_st1) ||
			    (0x0 == data_st1) || (0x1 == data_st1))) {
				if (data_st0 != data_st1) {
					data_diff = 1;
					break;
				}
			} else {
				data_err = 1;
			}
		}
		if (data_err)
			sprintf(dm[i], "d%d %s", i, "error");
		else if (data_diff)
			sprintf(dm[i], "d%d %s", i, "trans");
		else {
			if (data_st0 == 0x5)
				sprintf(dm[i], "d%d %s", i, "HS");
			if (data_st0 == 0x0)
				sprintf(dm[i], "d%d %s", i, "LP00");
			if (data_st0 == 0x3)
				sprintf(dm[i], "d%d %s", i, "LP11");
			if (data_st0 == 0x1)
				sprintf(dm[i], "d%d %s", i, "LP01");
		}
	}

	sprintf(data_mode, "%s %s %s %s", dm[0], dm[1], dm[2], dm[3]);

	printk(KERN_DEBUG "mipi dphy debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %9s %10s %9s %15s %12s %13s %12s %40s\n",
		"dphy_en",
		"set_lanes",
		"real_lanes",
		"rxclk_en",
		"rxdata_en",
		"set_mipi_bps",
		"real_mipi_bps",
		"clk_mode",
		"data_mode");

	printk(KERN_DEBUG "%7d %9d %10d %9d %15s %11dM %12dM %12s %40s\n",
		dphy_en,
		dev->mipi_para.lane_num,
		lanes,
		rxclk_en,
		rxdata_en,
		dev->mipi_para.bps/1000000,
		mipi_bps,
		clk_mode,
		data_mode);

	printk(KERN_DEBUG "mipi dphy debug end--------------------------------"
		"---------------------------------------------------\n");
}

void mipi_csi2_reg(struct vfe_dev *dev)
{
	void __iomem *csi2_reg = dev->regs.csi_regs + 0x1000;
	u32 i = 0;
	u32 rval;
	u32 csi2_en, reset, unpack, lanes;
	char *int_status = NULL;
	u32 set_fmt, real_fmt;
	u32 data_type, vir_ch = 0, packet_len = 0;

	u32 ecc_cal = 0, ecc_rx = 0, ecc_err = 0;
	u32 sum_cal = 0, sum_rx = 0, sum_err = 0;

	rval = vfe_reg_readl(csi2_reg);
	csi2_en = rval & 0x00000001;
	reset = (rval & 0x80000000) >> 31;
	unpack = (rval & 0x00000002) >> 1;

	rval = vfe_reg_readl(csi2_reg + 0x4);
	lanes = (rval & 0x3) + 1;

	rval = vfe_reg_readl(csi2_reg + 0x8);
	real_fmt = rval & 0x3f;
	set_fmt = dev->mipi_fmt.packet_fmt[0];

	vfe_reg_writel(csi2_reg + 0x60, 0x000e0000);
	usleep_range(1000, 1200);
	rval = vfe_reg_readl(csi2_reg + 0x70);
	data_type = rval & 0x3f;
	vir_ch = (rval & 0xc0) >> 6;
	packet_len = (rval & 0xffff0000) >> 16;

	for (i = 0; i < 3; i++) {
		vfe_reg_writel(csi2_reg + 0x58, 0xffffffff);
		usleep_range(1000, 1200);
	}
	rval = vfe_reg_readl(csi2_reg + 0x58);
	int_status = "ok";
	if ((rval == 0x00030300) || (rval == 0x00070300) ||
	    (rval == 0x00030f00) || (rval == 0x00070f00)) {
		int_status = "ok";
	} else {
		if (rval & 0x20000000)
			int_status = "RX count not match";
		if (rval & 0x10000000)
			int_status = "checksum error";
		if (rval & 0x08000000)
			int_status = "ECC single bit error";
		if (rval & 0x04000000)
			int_status = "ECC multi bit error";
		if (rval & 0x02000000)
			int_status = "LS and LE packet not match";
		if (rval & 0x01000000)
			int_status = "FS and FE packet not match";
		if (rval & 0x00000001)
			int_status = "fifo overflow";
	}
	for (i = 0; i < 10; i++) {
		rval = vfe_reg_readl(csi2_reg + 0x74);
		ecc_cal = (rval & 0x00ff0000) >> 16;
		ecc_rx = (rval & 0x000000ff);
		if (ecc_rx != ecc_cal) {
			ecc_err = 1;
			break;
		}
	}

	for (i = 0; i < 10; i++) {
		rval = vfe_reg_readl(csi2_reg + 0x78);
		sum_cal = (rval & 0xffff0000) >> 16;
		sum_rx = (rval & 0x0000ffff);
		if (sum_cal != sum_rx) {
			sum_err = 1;
			break;
		}
	}

	printk(KERN_DEBUG "mipi csi2 debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %5s %6s %5s %7s %8s %9s %6s %10s %10s %9s %9s\n",
		"csi2_en",
		"reset",
		"unpack",
		"lanes",
		"set_fmt",
		"real_fmt",
		"data_type",
		"vir_ch",
		"packet_len",
		"int_status",
		"check_ecc",
		"check_sum");

	printk(KERN_DEBUG "%7d %5d %6d %5d %7x %8x %9x %6d %10d %10s %9s %9s\n",
		csi2_en,
		reset,
		unpack,
		lanes,
		set_fmt,
		real_fmt,
		data_type,
		vir_ch,
		packet_len,
		int_status,
		ecc_err ? "error" : "ok",
		sum_err ? "error" : "ok");

	printk(KERN_DEBUG "mipi csi2 debug end--------------------------------"
		"---------------------------------------------------\n");
}

#elif defined CONFIG_ARCH_SUN9IW1P1
void mipi_dphy_reg(struct vfe_dev *dev)
{
	void __iomem *dphy_reg = dev->regs.csi_regs + 0x1000;
	u32 i = 0, j = 0;
	u32 rval;
	u32 sync_en, lanes, shutdown, dphy_reset, protocol_reset;
	char *clk_mode = NULL;
	char dm[4][10] = {"", "", "", ""}, data_mode[40] = "";

	u32 clk_st0 = 0, clk_st1 = 0, clk_diff = 0, clk_err = 0;
	u32 data_st0 = 0, data_st1 = 0, data_diff = 0;

	rval = vfe_reg_readl(dphy_reg + 0x4);
	sync_en = rval & 0x80000000;
	lanes = (rval & 0x00000003) + 1;

	rval = vfe_reg_readl(dphy_reg + 0x8);
	shutdown = rval & 0x00000001;

	rval = vfe_reg_readl(dphy_reg + 0xc);
	dphy_reset = rval & 0x00000001;

	rval = vfe_reg_readl(dphy_reg + 0x10);
	protocol_reset = rval & 0x00000001;

	for (i = 0; i < 50; i++) {
		rval = vfe_reg_readl(dphy_reg + 0x14);
		clk_st0 = (rval & 0x00000700) >> 8;
		usleep_range(1000, 1200);
		rval = vfe_reg_readl(dphy_reg + 0x14);
		clk_st1 = (rval & 0x00000700) >> 8;
		if (((0x3 == clk_st0) || (0x6 == clk_st0)) &&
		    ((0x3 == clk_st1) || (0x6 == clk_st1))) {
			if (clk_st0 != clk_st1) {
				clk_diff = 1;
				break;
			}
		} else {
			clk_err = 1;
		}
	}
	if (clk_err)
		clk_mode = "error";
	else if (clk_diff)
		clk_mode = "uncontinuous";
	else if (0x3 == clk_st0)
		clk_mode = "continuous";
	else
		clk_mode = "LP11";

	for (i = 0; i < lanes; i++) {
		for (j = 0; j < 50; j++) {
			rval = vfe_reg_readl(dphy_reg + 0x14);
			data_st0 = rval & (0x10 << i);
			usleep_range(1000, 1200);
			rval = vfe_reg_readl(dphy_reg + 0x14);
			data_st1 = rval & (0x10 << i);
			if (data_st0 != data_st1) {
				data_diff = 1;
				break;
			}
		}

		if (data_diff)
			sprintf(dm[i], "d%d %s", i, "trans");
		else {
			if (data_st0 == 0)
				sprintf(dm[i], "d%d %s", i, "HS");
			else
				sprintf(dm[i], "d%d %s", i, "LP11");

		}
	}
	sprintf(data_mode, "%s %s %s %s\n", dm[0], dm[1], dm[2], dm[3]);

	printk(KERN_DEBUG "mipi dphy debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %9s %10s %8s %10s %14s %8s %40s\n",
		"sync_en",
		"set_lanes",
		"real_lanes",
		"shutdown",
		"dphy_reset",
		"protocol_reset",
		"clk_mode",
		"data_mode");

	printk(KERN_DEBUG "%7d %9d %10d %8d %10d %14d %8s %40s\n",
		sync_en,
		dev->mipi_para.lane_num,
		lanes,
		shutdown,
		dphy_reset,
		protocol_reset,
		clk_mode,
		data_mode);

	printk(KERN_DEBUG "mipi dphy debug end--------------------------------"
		"---------------------------------------------------\n");
}

void mipi_csi2_reg(struct vfe_dev *dev)
{
	void __iomem *csi2_reg = dev->regs.csi_regs + 0x1000;
	u32 i = 0;
	u32 rval;
	u32 set_fmt = 0, real_fmt = 0;
	u32 err_flag = 0;

	rval = vfe_reg_readl(csi2_reg + 0x18);
	real_fmt = rval & 0x3f;
	set_fmt = dev->mipi_fmt.packet_fmt[0];

	vfe_reg_writel(csi2_reg + 0x20, 0xffffffff);
	vfe_reg_writel(csi2_reg + 0x24, 0xffffffff);

	for (i = 0; i < 5; i++) {
		if (vfe_reg_readl(csi2_reg + 0x20)) {
			err_flag = 1;
			break;
		}
		if (vfe_reg_readl(csi2_reg + 0x24)) {
			err_flag = 1;
			break;
		}
		usleep_range(1000, 1200);
	}

	printk(KERN_DEBUG "mipi csi2 debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %8s %10s\n",
		"set_fmt",
		"real_fmt",
		"err_status");

	printk(KERN_DEBUG "%7x %8x %10s\n",
		set_fmt,
		real_fmt,
		err_flag ? "error" : "ok");

	printk(KERN_DEBUG "mipi csi2 debug end--------------------------------"
		"---------------------------------------------------\n");
}

#elif defined CONFIG_ARCH_SUN8IW6P1
void mipi_dphy_reg(struct vfe_dev *dev)
{
	void __iomem *dphy_reg = dev->regs.csi_regs + 0x1000;
	u32 i = 0, j = 0;
	u32 rval;
	u32 sync_en, lanes, shutdown, dphy_reset, protocol_reset;
	char *clk_mode = NULL;
	char dm[4][10] = {"", "", "", ""}, data_mode[40] = "";

	u32 clk_st0 = 0, clk_st1 = 0, clk_diff = 0, clk_err = 0;
	u32 data_st0 = 0, data_st1 = 0, data_diff = 0;

	rval = vfe_reg_readl(dphy_reg + 0x100);
	lanes = ((rval & 0x00000030) >> 4) + 1;
	sync_en = (rval & 0x80000000) >> 31;

	rval = vfe_reg_readl(dphy_reg + 0x10);
	shutdown = (rval & 0x00008000) >> 15;
	dphy_reset = (rval & 0x80000000) >> 31;

	rval = vfe_reg_readl(dphy_reg + 0x4);
	protocol_reset = (rval & 0x80000000) >> 31;

	for (i = 0; i < 50; i++) {
		rval = vfe_reg_readl(dphy_reg + 0x14);
		clk_st0 = (rval & 0x00000700) >> 8;
		usleep_range(1000, 1200);
		rval = vfe_reg_readl(dphy_reg + 0x14);
		clk_st1 = (rval & 0x00000700) >> 8;
		if (((0x3 == clk_st0) || (0x6 == clk_st0)) &&
		    ((0x3 == clk_st1) || (0x6 == clk_st1))) {
			if (clk_st0 != clk_st1) {
				clk_diff = 1;
				break;
			}
		} else {
			clk_err = 1;
		}
	}

	if (clk_err)
		clk_mode = "error";
	else if (clk_diff)
		clk_mode = "uncontinuous";
	else if (0x3 == clk_st0)
		clk_mode = "continuous";
	else
		clk_mode = "LP11";

	for (i = 0; i < lanes; i++) {
		for (j = 0; j < 50; j++) {
			rval = vfe_reg_readl(dphy_reg + 0x14);
			data_st0 = rval & (0x10 << i);
			usleep_range(1000, 1200);
			rval = vfe_reg_readl(dphy_reg + 0x14);
			data_st1 = rval & (0x10 << i);
			if (data_st0 != data_st1) {
				data_diff = 1;
				break;
			}
		}

		if (data_diff)
			sprintf(dm[i], "d%d %s", i, "trans");
		else {
			if (data_st0 == 0)
				sprintf(dm[i], "d%d %s", i, "HS");
			else
				sprintf(dm[i], "d%d %s", i, "LP11");

		}
	}
	sprintf(data_mode, "%s %s %s %s\n", dm[0], dm[1], dm[2], dm[3]);

	printk(KERN_DEBUG "mipi dphy debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %9s %10s %8s %10s %14s %8s %40s\n",
		"sync_en",
		"set_lanes",
		"real_lanes",
		"shutdown",
		"dphy_reset",
		"protocol_reset",
		"clk_mode",
		"data_mode");

	printk(KERN_DEBUG "%7d %9d %10d %8d %10d %14d %8s %40s\n",
		sync_en,
		dev->mipi_para.lane_num,
		lanes,
		shutdown,
		dphy_reset,
		protocol_reset,
		clk_mode,
		data_mode);

	printk(KERN_DEBUG "mipi dphy debug end--------------------------------"
		"---------------------------------------------------\n");
}

void mipi_csi2_reg(struct vfe_dev *dev)
{
	void __iomem *csi2_reg = dev->regs.csi_regs + 0x1000;
	u32 i = 0;
	u32 rval;
	u32 set_fmt = 0, real_fmt = 0;
	u32 err_flag = 0;

	rval = vfe_reg_readl(csi2_reg + 0x104);
	real_fmt = rval & 0x3f;
	set_fmt = dev->mipi_fmt.packet_fmt[0];

	vfe_reg_writel(csi2_reg + 0x20, 0xffffffff);
	vfe_reg_writel(csi2_reg + 0x24, 0xffffffff);

	for (i = 0; i < 5; i++) {
		if (vfe_reg_readl(csi2_reg + 0x20)) {
			err_flag = 1;
			break;
		}
		if (vfe_reg_readl(csi2_reg + 0x24)) {
			err_flag = 1;
			break;
		}
		usleep_range(1000, 1200);
	}
	printk(KERN_DEBUG "mipi csi2 debug start------------------------------"
		"---------------------------------------------------\n");
	printk(KERN_DEBUG "%7s %8s %10s\n",
		"set_fmt",
		"real_fmt",
		"err_status");

	printk(KERN_DEBUG "%7x %8x %10s\n",
		set_fmt,
		real_fmt,
		err_flag ? "error" : "ok");

	printk(KERN_DEBUG "mipi csi2 debug end--------------------------------"
		"---------------------------------------------------\n");
}
#else
void mipi_dphy_reg(struct vfe_dev *dev)
{
	printk(KERN_DEBUG "this platform is not support mipi dphy debug\n");
}
void mipi_csi2_reg(struct vfe_dev *dev)
{
	printk(KERN_DEBUG "this platform is not support mipi csi2 debug\n");
}
#endif

static ssize_t mipi_dphy_dbg_show(struct device *d,
		    struct device_attribute *attr, char *buf)
{
	struct vfe_dev *dev = (struct vfe_dev *)dev_get_drvdata(d);
	mipi_dphy_reg(dev);
	return sprintf(buf, "%d\n", mipi_dphy_dbg);
}

static ssize_t mipi_dphy_dbg_store(struct device *d,
		    struct device_attribute *attr,
		    const char *buf, size_t count)
{
	int err;
	unsigned long val;

	err = strict_strtoul(buf, 10, &val);
	if (err) {
		printk("%s Invalid size\n", __func__);
		return err;
	}

	if (val < 0 || val > 1)
		printk("Invalid value, 0~1 is expected!\n");
	else
		mipi_dphy_dbg = val;

	return count;
}

static ssize_t mipi_csi2_dbg_show(struct device *d,
		    struct device_attribute *attr, char *buf)
{
	struct vfe_dev *dev = (struct vfe_dev *)dev_get_drvdata(d);
	mipi_csi2_reg(dev);
	return sprintf(buf, "%d\n", mipi_csi2_dbg);
}

static ssize_t mipi_csi2_dbg_store(struct device *d,
		    struct device_attribute *attr,
		    const char *buf, size_t count)
{
	int err;
	unsigned long val;

	err = strict_strtoul(buf, 10, &val);
	if (err) {
		printk("%s Invalid size\n", __func__);
		return err;
	}

	if (val < 0 || val > 1)
		printk("Invalid value, 0~1 is expected!\n");
	else
		mipi_csi2_dbg = val;

	return count;
}

struct device_attribute dbg_device_attrs[] = {
	__ATTR(mipi_dphy_dbg, S_IRUGO | S_IWUSR | S_IWGRP,
		mipi_dphy_dbg_show, mipi_dphy_dbg_store),
	__ATTR(mipi_csi2_dbg, S_IRUGO | S_IWUSR | S_IWGRP,
		mipi_csi2_dbg_show, mipi_csi2_dbg_store),
};

int camera_dbg_register(struct vfe_dev *dev)
{
	int i, ret;

	/* sysfs entries */
	for (i = 0; i < ARRAY_SIZE(dbg_device_attrs); i++) {
		ret = device_create_file(&dev->pdev->dev,
					&dbg_device_attrs[i]);
		if (ret) {
			printk("device_create_file error!\n");
			device_remove_file(&dev->pdev->dev,
				&dbg_device_attrs[i]);
		}
	}
	return 0;
}

int camera_dbg_unregister(struct vfe_dev *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(dbg_device_attrs); i++)
		device_remove_file(&dev->pdev->dev, &dbg_device_attrs[i]);

	return 0;
}

