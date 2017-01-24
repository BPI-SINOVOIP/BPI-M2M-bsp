/*
 * Copyright (c) 2013 Espressif System.
 *
 *  sdio stub code for allwinner
 */

extern void sunxi_mci_rescan_card(unsigned id, unsigned insert);
extern int rf_pm_gpio_ctrl(char *name, int level);
extern void wifi_pm_power(int on);

#define SDIO_ID 1

void sif_platform_rescan_card(unsigned insert)
{
	printk("%s id %d %u\n", __func__, SDIO_ID, insert);
	sunxi_mci_rescan_card(SDIO_ID, insert);
}

void sif_platform_reset_target(void)
{
	wifi_pm_power(0);
	mdelay(100);
	wifi_pm_power(1);
	
	mdelay(100);
}

void sif_platform_target_poweroff(void)
{
	mdelay(100);
    wifi_pm_power(0);
	mdelay(100);
}

void sif_platform_target_poweron(void)
{
    rf_pm_gpio_ctrl("chip_en",1);
	mdelay(100);

	wifi_pm_power(0);
	mdelay(100);
	wifi_pm_power(1);
	mdelay(100);
}

void sif_platform_target_speed(int high_speed)
{
	wifi_pm_power(high_speed);
}

#ifdef MMC_NO_CHANGE
extern int sunxi_mci_check_r1_ready(struct mmc_host* mmc, unsigned ms);
void sif_platform_check_r1_ready(struct esp_pub *epub)
{
        struct esp_sdio_ctrl *sctrl = NULL;
        struct sdio_func *func = NULL;
	int err;

	if (epub == NULL) {
        	ESSERT(epub != NULL);
		return;
	}
        sctrl = (struct esp_sdio_ctrl *)epub->sif;
        func = sctrl->func;
	if (func == NULL) {
        	ESSERT(func != NULL);
		return;
	}

	err = sunxi_mci_check_r1_ready(func->card->host, 1000);
        if (err != 0)
          printk("%s data timeout.\n", __func__);
}
#else
void sif_platform_check_r1_ready(struct esp_pub *epub)
{
}
#endif

#ifdef ESP_ACK_INTERRUPT
extern void sdmmc_ack_interrupt(struct mmc_host *mmc);
void sif_platform_ack_interrupt(struct esp_pub *epub)
{
        struct esp_sdio_ctrl *sctrl = NULL;
        struct sdio_func *func = NULL;

	if (epub == NULL) {
        	ESSERT(epub != NULL);
		return;
	}
        sctrl = (struct esp_sdio_ctrl *)epub->sif;
        func = sctrl->func;
	if (func == NULL) {
        	ESSERT(func != NULL);
		return;
	}

        sdmmc_ack_interrupt(func->card->host);
}
#endif //ESP_ACK_INTERRUPT

module_init(esp_sdio_init);
module_exit(esp_sdio_exit);
