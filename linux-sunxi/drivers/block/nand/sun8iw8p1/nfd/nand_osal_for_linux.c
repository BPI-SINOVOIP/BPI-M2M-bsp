/*
*********************************************************************************************************
*											        eBIOS
*						            the Easy Portable/Player Develop Kits
*									           dma sub system
*
*						        (c) Copyright 2006-2008, David China
*											All	Rights Reserved
*
* File    : clk_for_nand.c
* By      : Richard
* Version : V1.00
*********************************************************************************************************
*/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/spinlock.h>
#include <linux/hdreg.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/mutex.h>
/*#include <mach/clock.h>*/
#include <mach/platform.h>
/*#include <mach/hardware.h>*/
#include <mach/sys_config.h>
#include <linux/dma-mapping.h>
/*#include <mach/dma.h>*/
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/cacheflush.h>
#include <mach/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <linux/gpio.h>
/*#include "nand_lib.h"*/
#include "nand_blk.h"
/*#include <linux/regulator/consumer.h>*/
#include <mach/sunxi-chip.h>

#ifdef CONFIG_DMA_ENGINE
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dma/sunxi-dma.h>
#endif


#define  NAND_DRV_VERSION_0		0x2
#define  NAND_DRV_VERSION_1		0x35
#define  NAND_DRV_DATE			0x20180115
#define  NAND_DRV_TIME			0x1600

struct clk *pll6;
struct clk *nand0_clk;
struct clk *ahb_nand0;
/*struct regulator *regu1 = NULL;*/

int seq = 0;
int nand_handle = 0;

static __u32 PRINT_LEVEL = 0;

__u32 Dma_Map_Addr = 0;
extern struct completion spinand_dma_done;

struct dma_chan *dma_hdl_tx = NULL;
struct dma_chan *dma_hdl_rx = NULL;

extern __u32 SPIC_IO_BASE;
#define SPIC_PHYSIC_BASE   			0x01C68000
#define SPI_TX_IO_DATA             (SPIC_PHYSIC_BASE + 0x200)
#define SPI_RX_IO_DATA             (SPIC_PHYSIC_BASE + 0x300)

extern void *NAND_Malloc(unsigned int Size);
extern void NAND_Free(void *pAddr, unsigned int Size);
extern __u32 NAND_GetNdfcVersion(void);
__u32 NAND_Print_level(void);
int NAND_Print_DBG(const char *fmt, ...);


/*
*********************************************************************************************************
*                                               DMA TRANSFER END ISR
*
* Description: dma transfer end isr.
*
* Arguments  : none;
*
* Returns    : EPDK_TRUE/ EPDK_FALSE
*********************************************************************************************************
*/
int NAND_ClkRequest(__u32 nand_index)
{
	long rate;

	pll6 = clk_get(NULL, "pll6");
	if (NULL == pll6 || IS_ERR(pll6)) {
		printk("%s: pll6 clock handle invalid!\n", __func__);
		return -1;
	}

	rate = clk_get_rate(pll6);
	NAND_Print_DBG("%s: get pll6 rate %dHZ\n", __func__, (__u32)rate);

	if (nand_index == 0) {

		nand0_clk = clk_get(NULL, "spi0");

		if (NULL == nand0_clk || IS_ERR(nand0_clk)) {
			printk("%s: nand0 clock handle invalid!\n", __func__);
			return -1;
		}

		if (clk_set_parent(nand0_clk, pll6))
			printk("%s: set nand0_clk parent to pll6 failed!\n", __func__);

		rate = clk_round_rate(nand0_clk, 60000000);
		if (clk_set_rate(nand0_clk, rate))
			printk("%s: set nand0_clk rate to %dHZ failed!\n", __func__, (__u32)rate);

		if (clk_prepare_enable(nand0_clk))
			printk("%s: enable nand0_clk failed!\n", __func__);
	} else {
		printk("NAND_ClkRequest, nand_index error: 0x%x\n", nand_index);
		return -1;
	}

	return 0;
}

void NAND_ClkRelease(__u32 nand_index)
{
	if (nand_index == 0) {
		if (NULL != nand0_clk && !IS_ERR(nand0_clk)) {

			clk_disable_unprepare(nand0_clk);

			clk_put(nand0_clk);
			nand0_clk = NULL;
		}
	} else {
		printk("NAND_ClkRequest, nand_index error: 0x%x\n", nand_index);
	}

	if (NULL != pll6 && !IS_ERR(pll6)) {
		clk_put(pll6);
		pll6 = NULL;
	}

}

int NAND_SetClk(__u32 nand_index, __u32 nand_clk0)
{
	long    rate;

	if (nand_index == 0) {

		if (NULL == nand0_clk || IS_ERR(nand0_clk)) {
			printk("%s: clock handle invalid!\n", __func__);
			return -1;
		}

		rate = clk_round_rate(nand0_clk, nand_clk0 * 2000000);
		if (clk_set_rate(nand0_clk, rate))
			printk("%s: set nand0_clk rate to %dHZ failed! nand_clk: 0x%x\n", __func__, (__u32)rate, nand_clk0);
	} else {
		printk("NAND_SetClk, nand_index error: 0x%x\n", nand_index);
		return -1;
	}

	return 0;
}

int NAND_GetClk(__u32 nand_index, __u32 *pnand_clk0)
{
	long rate;

	if (nand_index == 0) {
		if (NULL == nand0_clk || IS_ERR(nand0_clk)) {
			printk("%s: clock handle invalid!\n", __func__);
			return -1;
		}
		rate = clk_get_rate(nand0_clk);
	} else {
		printk("NAND_GetClk, nand_index error: 0x%x\n", nand_index);
		return -1;
	}

	*pnand_clk0 = (rate / 2000000);

	return 0;
}

void eLIBs_CleanFlushDCacheRegion_nand(void *adr, size_t bytes)
{
	__cpuc_flush_dcache_area(adr, bytes + (1 << 5) * 2 - 2);
}


__s32 NAND_CleanFlushDCacheRegion(__u32 buff_addr, __u32 len)
{
	/*eLIBs_CleanFlushDCacheRegion_nand((void *)buff_addr, (size_t)len);*/
	return 0;
}


/*__u32 tmp_dma_phy_addr[2] = {0, 0};*/
/*__u32 tmp_dma_len[2] = {0, 0};*/

__u32 NAND_DMASingleMap(__u32 rw, __u32 buff_addr, __u32 len)
{
	__u32 mem_addr;
/*    __u32 nand_index = NAND_GetCurrentCH();*/

	if (rw == 1) {
		mem_addr = (__u32)dma_map_single(NULL, (void *)buff_addr, len, DMA_TO_DEVICE);
	} else {
		mem_addr = (__u32)dma_map_single(NULL, (void *)buff_addr, len, DMA_FROM_DEVICE);
	}

/*	tmp_dma_phy_addr[nand_index] = mem_addr;*/
/*	tmp_dma_len[nand_index] = len;*/

	return mem_addr;
}

__u32 NAND_DMASingleUnmap(__u32 rw, __u32 buff_addr, __u32 len)
{
	__u32 mem_addr = buff_addr;
/*	__u32 nand_index = NAND_GetCurrentCH();*/

/*    if(tmp_dma_phy_addr[nand_index]!=mem_addr)*/
/*    {*/
/*    	printk("NAND_DMASingleUnmap, dma addr not match! nand_index: 0x%x, tmp_dma_phy_addr:0x%x, mem_addr: 0x%x\n", nand_index,tmp_dma_phy_addr[nand_index], mem_addr);*/
/*	mem_addr = tmp_dma_phy_addr[nand_index];*/
/*    }*/
/**/
/*    if(tmp_dma_len[nand_index] != len)*/
/*    {*/
/*	    printk("NAND_DMASingleUnmap, dma len not match! nand_index: 0x%x, tmp_dma_len:0x%x, len: 0x%x\n", nand_index,tmp_dma_len[nand_index], len);*/
/*	    len = tmp_dma_len[nand_index];*/
/*	}*/


	if (rw == 1) {
		dma_unmap_single(NULL, (dma_addr_t)mem_addr, len, DMA_TO_DEVICE);
	} else {
		dma_unmap_single(NULL, (dma_addr_t)mem_addr, len, DMA_FROM_DEVICE);
	}

	return mem_addr;
}

__s32 NAND_AllocMemoryForDMADescs(__u32 *cpu_addr, __u32 *phy_addr)
{
	void *p = NULL;
	__u32 physical_addr  = 0;

#if 1

	/*void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp);*/
	p = (void *)dma_alloc_coherent(NULL, PAGE_SIZE,
				       (dma_addr_t *)&physical_addr, GFP_KERNEL);

	if (p == NULL) {
		printk("NAND_AllocMemoryForDMADescs(): alloc dma des failed\n");
		return -1;
	} else {
		*cpu_addr = (__u32)p;
		*phy_addr = physical_addr;
		NAND_Print_DBG("NAND_AllocMemoryForDMADescs(): cpu: 0x%x    physic: 0x%x\n",
			       *cpu_addr, *phy_addr);
	}

#else

	p = (void *)NAND_Malloc(1024);

	if (p == NULL) {
		printk("NAND_AllocMemoryForDMADescs(): alloc dma des failed\n");
		return -1;
	} else {
		*cpu_addr = (__u32)p;
		*phy_addr = (__u32)p;
		printk("NAND_AllocMemoryForDMADescs(): cpu: 0x%x    physic: 0x%x\n",
		       *cpu_addr, *phy_addr);
	}

#endif

	return 0;
}

__s32 NAND_FreeMemoryForDMADescs(__u32 *cpu_addr, __u32 *phy_addr)
{
#if 1

	/*void dma_free_coherent(struct device *dev, size_t size, void *cpu_addr, dma_addr_t handle);*/
	/*dma_free_coherent(NULL, PAGE_SIZE, (void *)(*cpu_addr), *phy_addr);*/

#else

	NAND_Free((void *)(*cpu_addr), 1024);

#endif

	*cpu_addr = 0;
	*phy_addr = 0;

	return 0;
}

#if 0
#ifdef __LINUX_SUPPORT_DMA_INT__
void NAND_EnDMAInt(void)
{
	__u32 nand_index;

	nand_index = NAND_GetCurrentCH();
	if (nand_index > 1)
		printk("NAND_ClearDMAInt, nand_index error: 0x%x\n", nand_index);

	/*clear interrupt*/
#if 0
	NFC_DmaIntClearStatus();
	if (NFC_DmaIntGetStatus()) {
		dbg_rbint_wrn("nand clear dma int status error in int enable \n");
		dbg_rbint_wrn("dma status: 0x%x\n", NFC_DmaIntGetStatus());
	}
#endif
	nanddma_ready_flag[nand_index] = 0;

	/*enable interrupt*/
	NFC_DmaIntEnable();

	dbg_rbint("dma int en\n");
}

void NAND_ClearDMAInt(void)
{
	__u32 nand_index;

	nand_index = NAND_GetCurrentCH();
	if (nand_index > 1)
		printk("NAND_ClearDMAInt, nand_index error: 0x%x\n", nand_index);

	/*disable interrupt*/
	NFC_DmaIntDisable();
	dbg_dmaint("ch %d dma int clear\n", nand_index);

	/*clear interrupt*/
	/*NFC_DmaIntClearStatus();*/
	/*if(NFC_DmaIntGetStatus())*/
	/*{*/
	/*	dbg_dmaint_wrn("nand clear dma int status error in int clear \n");*/
	/*	dbg_dmaint_wrn("dma status: 0x%x\n", NFC_DmaIntGetStatus());*/
	/*}*/

	nanddma_ready_flag[nand_index] = 0;
}

void NAND_DMAInterrupt(void)
{
	__u32 nand_index;

	nand_index = NAND_GetCurrentCH();
	if (nand_index > 1)
		printk("NAND_ClearDMAInt, nand_index error: 0x%x\n", nand_index);

	dbg_dmaint("ch %d dma int occor! \n", nand_index);
	if (!NFC_DmaIntGetStatus()) {
		dbg_dmaint_wrn("nand dma int late \n");
	}

	NAND_ClearDMAInt();

	nanddma_ready_flag[nand_index] = 1;
	if (nand_index == 0)
		wake_up(&NAND_DMA_WAIT_CH0);
	else if (nand_index == 1)
		wake_up(&NAND_DMA_WAIT_CH1);
}

__s32 NAND_WaitDmaFinish(void)
{
	__u32 nand_index;

	nand_index = NAND_GetCurrentCH();
	if (nand_index > 1)
		printk("NAND_ClearDMAInt, nand_index error: 0x%x\n", nand_index);

	NAND_EnDMAInt();

	/*wait_event(NAND_RB_WAIT, nandrb_ready_flag);*/
	dbg_dmaint("ch %d dma wait\n", nand_index);

	if (nanddma_ready_flag[nand_index]) {
		dbg_dmaint("ch %d fast dma int\n", nand_index);
		NAND_ClearDMAInt();
		return 0;
	}

	if (NFC_DmaIntGetStatus()) {
		dbg_dmaint("ch %d dma fast ready \n", nand_index);
		NAND_ClearDMAInt();
		return 0;
	}

	if (nand_index == 0) {
		if (wait_event_timeout(NAND_DMA_WAIT_CH0, nanddma_ready_flag[nand_index], 1 * HZ) == 0) {
			/*dbg_dmaint_wrn("+nand wait dma int time out, ch: 0x%d, dma_status: %x, dma_ready_flag: 0x%x\n", nand_index, NFC_DmaIntGetStatus(),nanddma_ready_flag[nand_index]);*/
			while (!NFC_DmaIntGetStatus())
				;
			/*dbg_dmaint_wrn("-nand wait dma int time out, ch: 0x%d, dma_status: %x, dma_ready_flag: 0x%x\n", nand_index, NFC_DmaIntGetStatus(),nanddma_ready_flag[nand_index]);*/
			NAND_ClearDMAInt();
		} else {
			dbg_dmaint("nand wait dma ready ok\n");
			NAND_ClearDMAInt();
		}
	} else if (nand_index == 1) {
		if (wait_event_timeout(NAND_DMA_WAIT_CH1, nanddma_ready_flag[nand_index], 1 * HZ) == 0) {
			/*dbg_dmaint_wrn("+nand wait dma int time out, ch: 0x%d, dma_status: %x, dma_ready_flag: 0x%x\n", nand_index, NFC_DmaIntGetStatus(),nanddma_ready_flag[nand_index]);*/
			while (!NFC_DmaIntGetStatus())
				;
			/*dbg_dmaint_wrn("-nand wait dma int time out, ch: 0x%d, dma_status: %x, dma_ready_flag: 0x%x\n", nand_index, NFC_DmaIntGetStatus(),nanddma_ready_flag[nand_index]);*/
			NAND_ClearDMAInt();
		} else {
			dbg_dmaint("nand wait dma ready ok\n");
			NAND_ClearDMAInt();
		}
	} else {
		NAND_ClearDMAInt();
		printk("NAND_WaitDmaFinish, error nand_index: 0x%x\n", nand_index);
	}

	return 0;
}

#else
__s32 NAND_WaitDmaFinish(void)
{

#if 0
	__u32 rw;
	__u32 buff_addr;
	__u32 len;

	wait_event(DMA_wait, nanddma_completed_flag);

	if (rw_flag == (__u32)NAND_READ)
		rw = 0;
	else
		rw = 1;
	buff_addr = dma_phy_address;
	len = dma_len_temp;
	NAND_DMASingleUnmap(rw, buff_addr, len);

	rw_flag = 0x1234;
#endif

	return 0;
}

#endif


void NAND_Interrupt(__u32 nand_index)
{
	if (nand_index > 1)
		printk("NAND_Interrupt, nand_index error: 0x%x\n", nand_index);
#ifdef __LINUX_NAND_SUPPORT_INT__

	/*printk("nand interrupt!\n");*/
#ifdef __LINUX_SUPPORT_RB_INT__
#if 0

	if (NFC_RbIntOccur()) {
		dbg_rbint("nand rb int\n");
		NAND_RbInterrupt();
	}
#else
	if (nand_index == 0) {
		if ((__NAND_REG(NAND_CH0_INT_EN_REG)&NAND_RB_INT_BITMAP) && (__NAND_REG(NAND_CH0_INT_ST_REG)&NAND_RB_INT_BITMAP))
			NAND_RbInterrupt();
	} else if (nand_index == 1) {
		if ((__NAND_REG(NAND_CH1_INT_EN_REG)&NAND_RB_INT_BITMAP) && (__NAND_REG(NAND_CH1_INT_ST_REG)&NAND_RB_INT_BITMAP))
			NAND_RbInterrupt();
	}
#endif
#endif

#ifdef __LINUX_SUPPORT_DMA_INT__
#if 0
	if (NFC_DmaIntOccur()) {
		dbg_dmaint("nand dma int\n");
		NAND_DMAInterrupt();
	}
#else
	if (nand_index == 0) {
		if ((__NAND_REG(NAND_CH0_INT_EN_REG)&NAND_DMA_INT_BITMAP) && (__NAND_REG(NAND_CH0_INT_ST_REG)&NAND_DMA_INT_BITMAP))
			NAND_DMAInterrupt();
	} else if (nand_index == 1) {
		if ((__NAND_REG(NAND_CH1_INT_EN_REG)&NAND_DMA_INT_BITMAP) && (__NAND_REG(NAND_CH1_INT_ST_REG)&NAND_DMA_INT_BITMAP))
			NAND_DMAInterrupt();
	}

#endif

#endif

#endif
}
#endif

__u32 NAND_VA_TO_PA(__u32 buff_addr)
{
	return (__u32)(__pa((void *)buff_addr));
}

void NAND_PIORequest(__u32 nand_index)
{

	script_item_u  *pin_list;
	int 		   pin_count;
	int 		   pin_index;

	PRINT_LEVEL = NAND_Print_level();

	/* get pin sys_config info */
	if (nand_index == 0) {
		pin_count = script_get_pio_list("spi0", &pin_list);
		NAND_Print_DBG("pin count:%d \n", pin_count);
	} else
		return ;
	if (pin_count == 0) {
		/* "lcd0" have no pin configuration */
		printk("pin count 0\n");
		return ;
	}

	/* request pin individually */
	for (pin_index = 0; pin_index < pin_count; pin_index++) {
		struct gpio_config *pin_cfg = &(pin_list[pin_index].gpio);
		char			   pin_name[SUNXI_PIN_NAME_MAX_LEN];
		unsigned long	   config;

		/* valid pin of sunxi-pinctrl,
		 * config pin attributes individually.
		 */
		sunxi_gpio_to_name(pin_cfg->gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg->mul_sel);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (pin_cfg->pull != GPIO_PULL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, pin_cfg->pull);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg->drv_level != GPIO_DRVLVL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DRV, pin_cfg->drv_level);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg->data != GPIO_DATA_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, pin_cfg->data);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
	}

	return ;
}

__s32 NAND_PIOFuncChange_DQSc(__u32 nand_index, __u32 en)
{
#if 0
	unsigned int 	ndfc_version;
	script_item_u  *pin_list;
	int 		   pin_count;
	int 		   pin_index;

	ndfc_version = NAND_GetNdfcVersion();
	if (ndfc_version == 1) {
		printk("NAND_PIOFuncChange_EnDQScREc: invalid ndfc version!\n");
		return 0;
	}

	/* get pin sys_config info */
	if (nand_index == 0)
		pin_count = script_get_pio_list("nand0", &pin_list);
	else if (nand_index == 1)
		pin_count = script_get_pio_list("nand1", &pin_list);
	else {
		pin_count = 0;
		printk("NAND_PIOFuncChange_DQSc, wrong nand index %d\n", nand_index);
	}

	if (pin_count == 0) {
		/* "lcd0" have no pin configuration */
		return 0;
	}

	{
		struct gpio_config *pin_cfg;
		char			   pin_name[SUNXI_PIN_NAME_MAX_LEN];
		unsigned long	   config;

		/* change pin func from CE3 to DQSc */
		pin_index = 18;
		if (pin_index > pin_count) {
			printk("NAND_PIOFuncChange_EnDQSc: pin_index error, %d/%d\n", pin_index, pin_count);
			return -1;
		}
		pin_cfg = &(pin_list[pin_index].gpio);

		sunxi_gpio_to_name(pin_cfg->gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg->mul_sel);

		if (en) {
			if ((config & 0xffff) == 0x3)
				printk("DQSc has already been enabled!\n");
			else if ((config & 0xffff) == 0x2) {
				config &= ~(0xffff);
				config |= 0x3;
				pin_config_set(SUNXI_PINCTRL, pin_name, config);
			} else {
				printk("NAND_PIOFuncChange_EnDQSc: wrong pin func status: %d %d\n", pin_index, (__u32)(config & 0xffff));
			}
		} else {
			if ((config & 0xffff) == 0x2)
				printk("DQSc has already been disenabled!\n");
			else if ((config & 0xffff) == 0x3) {
				config &= ~(0xffff);
				config |= 0x3;
				pin_config_set(SUNXI_PINCTRL, pin_name, config);
			} else {
				printk("NAND_PIOFuncChange_EnDQSc: wrong pin func status: %d %d\n", pin_index, (__u32)(config & 0xffff));
			}
		}
	}
#endif
	return 0;
}

__s32 NAND_PIOFuncChange_REc(__u32 nand_index, __u32 en)
{
#if 0
	unsigned int 	ndfc_version;
	script_item_u  *pin_list;
	int 		   pin_count;
	int 		   pin_index;

	ndfc_version = NAND_GetNdfcVersion();
	if (ndfc_version == 1) {
		printk("NAND_PIOFuncChange_EnDQScREc: invalid ndfc version!\n");
		return 0;
	}

	/* get pin sys_config info */
	if (nand_index == 0)
		pin_count = script_get_pio_list("nand0", &pin_list);
	else if (nand_index == 1)
		pin_count = script_get_pio_list("nand1", &pin_list);
	else {
		pin_count = 0;
		printk("NAND_PIOFuncChange_DQSc, wrong nand index %d\n", nand_index);
	}

	if (pin_count == 0) {
		/* "lcd0" have no pin configuration */
		return 0;
	}

	{
		struct gpio_config *pin_cfg;
		char			   pin_name[SUNXI_PIN_NAME_MAX_LEN];
		unsigned long	   config;

		/* change pin func from CE2 to REc */
		pin_index = 17;
		if (pin_index > pin_count) {
			printk("NAND_PIOFuncChange_EnREc: pin_index error, %d/%d\n", pin_index, pin_count);
			return -1;
		}
		pin_cfg = &(pin_list[pin_index].gpio);

		sunxi_gpio_to_name(pin_cfg->gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg->mul_sel);

		if (en) {
			if ((config & 0xffff) == 0x3)
				printk("REc has already been enabled!\n");
			else if ((config & 0xffff) == 0x2) {
				config &= ~(0xffff);
				config |= 0x3;
				pin_config_set(SUNXI_PINCTRL, pin_name, config);
			} else {
				printk("NAND_PIOFuncChange_EnREc: wrong pin func status: %d %d\n", pin_index, (__u32)(config & 0xffff));
			}
		} else {
			if ((config & 0xffff) == 0x2)
				printk("REc has already been disenabled!\n");
			else if ((config & 0xffff) == 0x3) {
				config &= ~(0xffff);
				config |= 0x3;
				pin_config_set(SUNXI_PINCTRL, pin_name, config);
			} else {
				printk("NAND_PIOFuncChange_EnREc: wrong pin func status: %d %d\n", pin_index, (__u32)(config & 0xffff));
			}
		}
	}
#endif
	return 0;
}

void NAND_PIORelease(__u32 nand_index)
{

	int	cnt;
	script_item_u *list = NULL;

	if (nand_index == 0) {
		/*printk("[NAND] nand gpio_release\n");*/

		/* ??ȡgpio list */
		cnt = script_get_pio_list("spi0", &list);
		if (0 == cnt) {
			printk("get spi0_para gpio list failed\n");
			return;
		}

		/* ?ͷ?gpio */
		while (cnt--)
			gpio_free(list[cnt].gpio.gpio);
	} else {
		printk("NAND_PIORelease, nand_index error: 0x%x\n", nand_index);
	}
}

void NAND_Memset(void *pAddr, unsigned char value, unsigned int len)
{
	memset(pAddr, value, len);
}

void NAND_Memcpy(void *pAddr_dst, void *pAddr_src, unsigned int len)
{
	memcpy(pAddr_dst, pAddr_src, len);
}

void *NAND_Malloc(unsigned int Size)
{
	return kmalloc(Size, GFP_KERNEL);
}

void NAND_Free(void *pAddr, unsigned int Size)
{
	kfree(pAddr);
}

int NAND_Print(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

	return r;
}

int NAND_Print_DBG(const char *fmt, ...)
{

	va_list args;
	int r;

	if ((PRINT_LEVEL == 0) || (PRINT_LEVEL == 0xffffffff))
		return 0;
	else {
		va_start(args, fmt);
		r = vprintk(fmt, args);
		va_end(args);

		return r;
	}
}


void *NAND_IORemap(unsigned int base_addr, unsigned int size)
{
	return (void *)base_addr;
}

__u32 NAND_GetIOBaseAddr(void)
{
	return 0xf1c68000;
}

__u32 NAND_GetNdfcVersion(void)
{
	/*
		0:
		1: A31/A31s/A21/A23
		2:
	*/
	return 1;
}

__u32 NAND_GetNdfcDmaMode(void)
{

	return 1; /*idma*/

}

__u32 NAND_GetMaxChannelCnt(void)
{
	return 1;
}

__u32 NAND_GetPlatform(void)
{
	return 1681;
}

DEFINE_SEMAPHORE(nand_physic_mutex);

int NAND_PhysicLockInit(void)
{
	return 0;
}

int NAND_PhysicLock(void)
{
	down(&nand_physic_mutex);
	return 0;
}

int NAND_PhysicUnLock(void)
{
	up(&nand_physic_mutex);
	return 0;
}

int NAND_PhysicLockExit(void)
{
	return 0;
}

/* request dma channel and set callback function */
int nand_request_tx_dma(void)
{
	dma_cap_mask_t mask;

	printk("request tx DMA\n");

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	if (dma_hdl_tx == NULL) {
		dma_hdl_tx = dma_request_channel(mask, NULL, NULL);
		if (dma_hdl_tx == NULL) {
			printk("Request tx DMA failed!\n");
			return -EINVAL;
		}
	}

	return 0;
}

int nand_request_rx_dma(void)
{
	dma_cap_mask_t mask;

	printk("request rx DMA\n");

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	if (dma_hdl_rx == NULL) {
		dma_hdl_rx = dma_request_channel(mask, NULL, NULL);
		if (dma_hdl_rx == NULL) {
			printk("Request rx DMA failed!\n");
			return -EINVAL;
		}
	}

	return 0;
}

int NAND_ReleaseTxDMA(void)
{
	if (dma_hdl_tx != NULL) {
		printk("spinand release tx dma\n");
		dma_release_channel(dma_hdl_tx);
		dma_hdl_tx = NULL;
		return 0;
	}
	return 0;

}

int NAND_ReleaseRxDMA(void)
{
	if (dma_hdl_rx != NULL) {
		printk("spinand release rx dma\n");
		dma_release_channel(dma_hdl_rx);
		dma_hdl_rx = NULL;
		return 0;
	}
	return 0;

}

void nand_dma_callback(void *arg)
{
	complete(&spinand_dma_done);
}

int tx_dma_config_start(dma_addr_t addr, __u32 length)
{
	/*int ret = 0;*/
	/*int nents = 0;*/
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	dma_conf.direction = DMA_MEM_TO_DEV;
	dma_conf.dst_addr = SPI_TX_IO_DATA;

	dma_conf.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.src_maxburst = 8;
	dma_conf.dst_maxburst = 8;
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SPI0TX, DRQSRC_SDRAM);

	dmaengine_slave_config(dma_hdl_tx, &dma_conf);

	dma_desc = dmaengine_prep_slave_single(dma_hdl_tx, addr, length,
					       DMA_TO_DEVICE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!dma_desc) {
		printk("tx dmaengine prepare failed!\n");
		return -1;
	}

	dma_desc->callback = nand_dma_callback;
	dma_desc->callback_param = NULL;
	dmaengine_submit(dma_desc);

	dma_async_issue_pending(dma_hdl_tx);

	return 0;
}
int rx_dma_config_start(dma_addr_t addr, __u32 length)
{
	/*int ret = 0;*/
	/*int nents = 0;*/
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	dma_conf.direction = DMA_DEV_TO_MEM;
	dma_conf.src_addr = SPI_RX_IO_DATA;

	dma_conf.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.src_maxburst = 8;
	dma_conf.dst_maxburst = 8;
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SDRAM, DRQSRC_SPI0RX);

	dmaengine_slave_config(dma_hdl_rx, &dma_conf);

	dma_desc = dmaengine_prep_slave_single(dma_hdl_rx, addr, length,
					       DMA_FROM_DEVICE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!dma_desc) {
		printk("rx dmaengine prepare failed!\n");
		return -1;
	}

	dma_desc->callback = nand_dma_callback;
	dma_desc->callback_param = NULL;
	dmaengine_submit(dma_desc);

	dma_async_issue_pending(dma_hdl_rx);

	return 0;
}


int nand_dma_config_start(__u32 rw, __u32 addr, __u32 length)
{

	Dma_Map_Addr = NAND_DMASingleMap(rw, addr, length);

	if (rw)
		tx_dma_config_start(Dma_Map_Addr, length);
	else
		rx_dma_config_start(Dma_Map_Addr, length);

	return 0;
}

int NAND_WaitDmaFinish(__u32 tx_flag, __u32 rx_flag)
{
	__u32 timeout = 2000;
	__u32 timeout_flag = 0;

	if (tx_flag || rx_flag) {
		timeout_flag = wait_for_completion_timeout(&spinand_dma_done, msecs_to_jiffies(timeout));
		if (!timeout_flag) {
			printk("wait dma finish timeout!!\n");
			return -1;
		}
	}

	return 0;
}

int Nand_Dma_End(__u32 rw, __u32 addr, __u32 length)
{

	NAND_DMASingleUnmap(rw, Dma_Map_Addr, length);

	return 0;
}

__u32 NAND_GetNandExtPara(__u32 para_num)
{
	script_item_u nand_para;
	script_item_value_type_e type;

	if (para_num == 0) {
		type = script_get_item("spinand", "spinand_p0", &nand_para);
		if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
			NAND_Print_DBG("spinand, spinand_p0, nand type err! %d\n", type);
			return 0xffffffff;
		} else
			return nand_para.val;
	} else if (para_num == 1) {
		type = script_get_item("spinand", "spinand_p1", &nand_para);
		if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
			NAND_Print_DBG("spinand, spinand_p1, nand type err! %d\n", type);
			return 0xffffffff;
		} else
			return nand_para.val;
	} else {
		printk("NAND_GetNandExtPara: wrong para num: %d\n", para_num);
		return 0xffffffff;
	}
}

__u32 NAND_GetNandIDNumCtrl(void)
{
	script_item_u id_number_ctl;
	script_item_value_type_e type;

	type = script_get_item("spinand", "spinand_id_number_ctl", &id_number_ctl);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		NAND_Print_DBG("spinand, spinand_id_number_ctl, nand type err! %d\n", type);
		return 0x0;
	} else {
		NAND_Print_DBG("spinand : get id_number_ctl from script,%x \n", id_number_ctl.val);
		return id_number_ctl.val;
	}
}

static void dumphex32(char *name, char *base, int len)
{
	__u32 i;

	if (!((unsigned int)base & 0xf0000000)) {
		printk("dumphex32: err para in kernel, %s 0x%x\n", name, (unsigned int)base);
		return ;
	}
	printk("dump %s registers:", name);
	for (i = 0; i < len * 4; i += 4) {
		if (!(i & 0xf))
			printk("\n0x%p : ", base + i);
		printk("0x%08x ", *((volatile unsigned int *)(base + i)));
	}
	printk("\n");
}

void NAND_DumpReg(void)
{
	dumphex32("nand0 reg", (char *)0xf1c03000, 60);
	dumphex32("gpio reg", (char *)0xf1c20848, 40);
	dumphex32("clk reg", (char *)0xf1c20080, 8);
	dumphex32("dma reg part0", (char *)0xf1c02000, 8);
	dumphex32("dma reg part1", (char *)0xf1c02100, 50);
}

void NAND_ShowEnv(__u32 type, char *name, __u32 len, __u32 *val)
{
	int i;

	if (len && (val == NULL)) {
		printk("kernel:NAND_ShowEnv, para error!\n");
		return ;
	}

	if (type == 0) {
		printk("kernel:%s: ", name);
		for (i = 0; i < len; i++) {
			if (i && (i % 8 == 0))
				printk("\n");
			printk("%x ", val[i]);
		}
		printk("\n");
	} else {
		printk("kernel:NAND_ShowEnv, type error, %d!\n", type);
	}

	return ;
}

void NAND_Print_Version(void)
{
	__u32 val[4] = {0};

	val[0] = NAND_DRV_VERSION_0;
	val[1] = NAND_DRV_VERSION_1;
	val[2] = NAND_DRV_DATE;
	val[3] = NAND_DRV_TIME;
	NAND_ShowEnv(0, "nand version", 4, val);

}

int NAND_get_storagetype(void)
{
#if 1
	script_item_value_type_e script_ret;
	script_item_u storage_type;

	script_ret = script_get_item("target", "storage_type", &storage_type);
	if (script_ret != SCIRPT_ITEM_VALUE_TYPE_INT) {
		NAND_Print_DBG("nand init fetch storage_type failed\n");
		storage_type.val = 0;
		return storage_type.val;
	}

	return (int)storage_type.val;
#else
	return 1;
#endif

}


int NAND_GetVoltage(void)
{


	int ret = 0;
#if 0

	if (regu1 == NULL) {
		regu1 = regulator_get(NULL, "axp22_dcdc1");
		if (IS_ERR(regu1)) {
			printk("nand:some error happen, fail to get regulator axp22_dcdc1!");
			return -1;
		}

		/*set output voltage to 3.0V*/
		ret = regulator_set_voltage(regu1, 3000000, 3000000);
		if (IS_ERR(regu1)) {
			printk("nand:some error happen, fail to set regulator voltage axp22_dcdc1!");
			return -1;
		}

		/*enable regulator*/
		ret = regulator_enable(regu1);
		if (IS_ERR(regu1)) {
			printk("nand:some error happen, fail to enable regulator axp22_dcdc1!");
			return -1;
		}
		printk("nand:get voltage axp22_dcdc1 ok:%x\n", (__u32)regu1);
	} else
		printk("nand:has already get voltage\n");
#endif

	return ret;


}

int NAND_ReleaseVoltage(void)
{
	int ret = 0;

#if 0


	if (regu1 != NULL) {
		printk("nand release voltage axp22_dcdc1\n");

		ret = regulator_disable(regu1);
		if (ret)
			printk("nand:regulator disable fail ret is %x \n", ret);
		if (IS_ERR(regu1)) {
			printk("nand:some error happen, fail to disable regulator axp22_dcdc1!");
			return -1;
		}
		/*put regulator when module exit*/
		regulator_put(regu1);

		regu1 = NULL;
	} else {
		printk("nand had already release voltage axp22_dcdc1\n");
	}


#endif
	return ret;

}

int NAND_IS_Secure_sys(void)
{
	if (sunxi_soc_is_secure())
		return 0;
	else
		return -1;
}

__u32 NAND_Print_level(void)
{
	script_item_u print_level;
	script_item_value_type_e type;

	type = script_get_item("spinand", "spinand_print_level", &print_level);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		return 0xffffffff;
	} else
		return print_level.val;

}

void nand_cond_resched(void)
{
	cond_resched();
}
