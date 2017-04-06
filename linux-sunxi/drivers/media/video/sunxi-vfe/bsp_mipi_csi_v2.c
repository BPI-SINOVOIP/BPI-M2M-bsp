/*
 * sunxi mipi bsp interface
 * Author:wangxuan
 */
//#include "include.h"
#include "bsp_mipi_csi.h"

static void mipi_csi2_reg_default(unsigned int sel)
{	
	return;
}

static void mipi_dphy_init(unsigned int sel)
{
	return;
}

static void mipi_ctl_init(unsigned int sel)
{
	return;
}

static void mipi_ctl_enable(unsigned int sel)
{
	return;
}

static void mipi_ctl_disable(unsigned int sel)
{
	return;
}

static void mipi_s_lane(unsigned int sel, unsigned char lane_num)
{
	return;
}

static void mipi_s_ch_num(unsigned int sel, unsigned char ch_num)
{
	return;
}

static void mipi_s_pkt(unsigned int sel, unsigned char ch,unsigned char vc,enum pkt_fmt mipi_pkt_fmt)
{
	return;
}

void bsp_mipi_csi_set_version(unsigned int sel, unsigned int ver)
{
	return;
}

int bsp_mipi_csi_set_base_addr(unsigned int sel, unsigned int addr_base)
{

	return 0;
}

int bsp_mipi_dphy_set_base_addr(unsigned int sel, unsigned int addr_base)
{
	return 0;
}

void bsp_mipi_csi_dphy_init(unsigned int sel)
{
	return;
}

void bsp_mipi_csi_dphy_exit(unsigned int sel)
{
	return;
}

void bsp_mipi_csi_dphy_enable(unsigned int sel)
{
	return;
}

void bsp_mipi_csi_dphy_disable(unsigned int sel)
{
	return;
}

void bsp_mipi_csi_protocol_enable(unsigned int sel)
{
	return;
}

void bsp_mipi_csi_protocol_disable(unsigned int sel)
{
	return;
}

static void bsp_mipi_csi_set_dphy_timing(unsigned int sel, unsigned int *mipi_bps,unsigned int dphy_clk, unsigned int mode)
{
	return;
}

static void bsp_mipi_csi_set_lane(unsigned int sel, unsigned char lane_num)
{
	return;
}

static void bsp_mipi_csi_set_total_ch(unsigned int sel, unsigned char ch_num)
{
	return;
}

static void bsp_mipi_csi_set_pkt_header(unsigned int sel, unsigned char ch,unsigned char vc,enum pkt_fmt mipi_pkt_fmt)
{
	return;
}

void bsp_mipi_csi_set_para(unsigned int sel, struct mipi_para *para)
{
	return;
}

void bsp_mipi_csi_set_fmt(unsigned int sel, unsigned int total_rx_ch, struct mipi_fmt *fmt)
{   
	return;
}
    
