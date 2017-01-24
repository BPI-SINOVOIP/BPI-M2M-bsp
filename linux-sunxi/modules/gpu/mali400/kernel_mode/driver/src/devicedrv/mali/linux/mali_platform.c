#include <linux/mali/mali_utgard.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/clk/sunxi_name.h>
#include <linux/clk-private.h>
#include <linux/pm_runtime.h>
#include <linux/dma-mapping.h>
#include <mach/irqs.h>
#include <mach/sys_config.h>
#include <mach/platform.h>
#include "mali_kernel_common.h"

static struct clk *mali_clk = NULL;
static struct clk *gpu_pll  = NULL;
extern unsigned long totalram_pages;

struct __fb_addr_para {
	unsigned int fb_paddr;
	unsigned int fb_size;
};

extern void sunxi_get_fb_addr_para(struct __fb_addr_para *fb_addr_para);

static void mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data);

int mali_freq = 384; /* MHz */

static struct mali_gpu_device_data mali_gpu_data = {
	.utilization_interval = 500, /* 500ms */
	.utilization_callback = mali_gpu_utilization_callback,
};

static struct resource mali_gpu_resources[]= {
    MALI_GPU_RESOURCES_MALI400_MP2_PMU(SUNXI_GPU_PBASE, SUNXI_IRQ_GPUGP, SUNXI_IRQ_GPUGPMMU, \
                                        SUNXI_IRQ_GPUPP0, SUNXI_IRQ_GPUPPMMU0, SUNXI_IRQ_GPUPP1, SUNXI_IRQ_GPUPPMMU1)
};

static struct platform_device mali_gpu_device = {
    .name = MALI_GPU_NAME_UTGARD,
    .id = 0,
    .dev.coherent_dma_mask = DMA_BIT_MASK(32),
};

/*
***************************************************************
 @Function   :get_gpu_clk

 @Description:Get gpu related clocks

 @Input      :None

 @Return     :Zero or error code
***************************************************************
*/
static int get_gpu_clk(void)
{
	gpu_pll = clk_get(NULL, PLL_GPU_CLK);
	if (!gpu_pll || IS_ERR(gpu_pll)) {
		MALI_PRINT_ERROR(("Failed to get gpu pll clock!\n"));
		return -1;
	} 

	mali_clk = clk_get(NULL, GPU_CLK);
	if (!mali_clk || IS_ERR(mali_clk)) {
		MALI_PRINT_ERROR(("Failed to get mali clock!\n"));     
		return -1;
	}

	return 0;
}

/*
***************************************************************
 @Function   :set_freq

 @Description:Set the frequency of gpu related clocks

 @Input	     :Frequency value

 @Return     :Zero or error code
***************************************************************
*/
static int set_freq(int freq /* MHz */)
{
	if (clk_set_rate(gpu_pll, freq*1000*1000)) {
        MALI_PRINT_ERROR(("Failed to set gpu pll clock!\n"));
		return -1;
    }

	if(clk_set_rate(mali_clk, freq*1000*1000)) {
		MALI_PRINT_ERROR(("Failed to set mali clock!\n"));
		return -1;
	}

	mali_freq = freq;

	MALI_PRINT(("Set gpu frequency to %d MHz\n", mali_freq));

	return 0;
}

/*
***************************************************************
 @Function   :mali_set_freq

 @Description:Set the frequency of gpu related clocks with mali dvfs function

 @Input	     :Frequency value

 @Return     :Zero or error code
***************************************************************
*/
int mali_set_freq(int freq /* MHz */)
{
	int err;
	mali_dev_pause();
	err = set_freq(freq);
	mali_dev_resume();
	return err;
}

/*
***************************************************************
 @Function   :enable_gpu_clk

 @Description:Enable gpu related clocks

 @Input	     :None

 @Return     :None
***************************************************************
*/
void enable_gpu_clk(void)
{
	if (mali_clk->enable_count == 0) {
		if (clk_prepare_enable(gpu_pll))
			MALI_PRINT_ERROR(("Failed to enable gpu pll!\n"));

		if(clk_prepare_enable(mali_clk))
			MALI_PRINT_ERROR(("Failed to enable mali clock!\n"));
	}
}

/*
***************************************************************
 @Function   :disable_gpu_clk
	
 @Description:Disable gpu related clocks
	
 @Input	     :None

 @Return     :None
***************************************************************
*/
void disable_gpu_clk(void)
{
	if (mali_clk->enable_count == 1) {
		clk_disable_unprepare(mali_clk);
		clk_disable_unprepare(gpu_pll);
	}
}

/*
***************************************************************
 @Function   :parse_fex

 @Description:Parse fex file data of gpu

 @Input	     :None

 @Return     :None
***************************************************************
*/
static void parse_fex(void)
{
	script_item_u mali_clk_freq;
	script_item_value_type_e type;

	type = script_get_item("gpu_mali400_0", "freq", &mali_clk_freq);
	if (SCIRPT_ITEM_VALUE_TYPE_INT == type)
		if (mali_clk_freq.val > 0)
			mali_freq = mali_clk_freq.val;
}

/*
***************************************************************
 @Function   :mali_platform_init

 @Description:Init the power and clocks of gpu

 @Input	     :None

 @Return     :Zero or error code
***************************************************************
*/
static int mali_platform_init(void)
{
	parse_fex();

	if(get_gpu_clk())
		goto err_out;

	if(set_freq(mali_freq))
		goto err_out;

	enable_gpu_clk();

	MALI_PRINT(("Init Mali gpu successfully\n"));
    return 0;

err_out:
	MALI_PRINT_ERROR(("Failed to init Mali gpu!\n"));
	return -1;
}

/*
***************************************************************
 @Function   :mali_platform_device_unregister

 @Description:Unregister mali platform device

 @Input	     :None

 @Return     :Zero
***************************************************************
*/
int mali_platform_device_unregister(void)
{
	platform_device_unregister(&mali_gpu_device);

	disable_gpu_clk();

	return 0;
}

/*
***************************************************************
 @Function   :sun8i_mali_platform_device_register

 @Description:Register mali platform device

 @Input	     :None

 @Return     :Zero or error code
***************************************************************
*/
int mali_platform_device_register(void)
{
	int err;	
    struct __fb_addr_para fb_addr_para = {0};

    sunxi_get_fb_addr_para(&fb_addr_para);

    err = platform_device_add_resources(&mali_gpu_device, mali_gpu_resources, sizeof(mali_gpu_resources) / sizeof(mali_gpu_resources[0]));
    if (0 == err) {
        mali_gpu_data.fb_start = fb_addr_para.fb_paddr;
        mali_gpu_data.fb_size = fb_addr_para.fb_size;
		mali_gpu_data.shared_mem_size = totalram_pages * PAGE_SIZE; /* B */

        err = platform_device_add_data(&mali_gpu_device, &mali_gpu_data, sizeof(mali_gpu_data));
        if (0 == err) {
            err = platform_device_register(&mali_gpu_device);
            if (0 == err) {
                if(0 != mali_platform_init())
					return -1;
#if defined(CONFIG_PM_RUNTIME)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
				pm_runtime_set_autosuspend_delay(&(mali_gpu_device.dev), 1000);
				pm_runtime_use_autosuspend(&(mali_gpu_device.dev));
#endif
				pm_runtime_enable(&(mali_gpu_device.dev));
#endif /* CONFIG_PM_RUNTIME */

				return 0;
            }
        }

		mali_platform_device_unregister();
    }

    return err;
}

void mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data)
{
}
