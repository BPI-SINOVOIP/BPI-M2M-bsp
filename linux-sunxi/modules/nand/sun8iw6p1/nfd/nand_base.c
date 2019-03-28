#include "nand_blk.h"
#include "nand_dev.h"

/*****************************************************************************/

extern struct nand_blk_ops mytr;
extern struct _nand_info *p_nand_info;

extern int init_blklayer(void);
extern int init_blklayer_for_dragonboard(void);
extern void exit_blklayer(void);
extern void set_cache_level(struct _nand_info *nand_info,
			    unsigned short cache_level);
extern void set_capacity_level(struct _nand_info *nand_info,
			       unsigned short capacity_level);
extern __u32 nand_wait_rb_mode(void);
extern __u32 nand_wait_dma_mode(void);
extern void do_nand_interrupt(unsigned int no);
extern void print_nftl_zone(void *zone);
extern int nand_before_shutdown(void *zone);
extern int NAND_get_storagetype(void);
extern int NAND_Get_Dragonboard_Flag(void);
extern int nand_thread(void *arg);
extern int NAND_CheckBoot(void);

int test_mbr(uchar *data);
extern int NAND_Print_DBG(const char *fmt, ...);

#define BLK_ERR_MSG_ON

static unsigned int channel0;

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
spinlock_t nand_int_lock;

static irqreturn_t nand_interrupt(int irq, void *channel)
{
	unsigned int no;
	unsigned long iflags;

	spin_lock_irqsave(&nand_int_lock, iflags);

	no = *((unsigned int *)channel);

	do_nand_interrupt(no);

	spin_unlock_irqrestore(&nand_int_lock, iflags);

	return IRQ_HANDLED;
}

#if 0
/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
static int nand_early_suspend(void)
{
	struct _nftl_blk *nftl_blk;
	struct nand_blk_ops *tr = &mytr;

	nftl_blk = tr->nftl_blk_head.nftl_blk_next;

	nand_dbg_err("nand_early_suspend\n");
	while (nftl_blk != NULL) {
		nand_dbg_err("nand\n");
		mutex_lock(nftl_blk->blk_lock);
		nftl_blk->flush_write_cache(nftl_blk, 0xffff);
		mutex_unlock(nftl_blk->blk_lock);
		nftl_blk = nftl_blk->nftl_blk_next;
	}
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
static int nand_early_resume(void)
{
	nand_dbg_err("nand_early_resume\n");
	return 0;
}
#endif
/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
extern int nand_clean_zone_table(void *p);
static int nand_suspend(struct platform_device *plat_dev, pm_message_t state)
{
#if 0
	if (NORMAL_STANDBY == standby_type) {
		nand_dbg_err("[NAND] nand_suspend normal\n");

		NandHwNormalStandby();
	} else if (SUPER_STANDBY == standby_type) {
		nand_dbg_err("[NAND] nand_suspend super\n");
		NandHwSuperStandby();
	}

	nand_dbg_err("[NAND] nand_suspend ok\n");
#else
	nand_dbg_err("[NAND] nand_suspend\n");
	NandHwSuperStandby();
	nand_dbg_err("[NAND] nand_suspend ok\n");
#endif
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
extern int nand_find_zone_table(void *p);
static int nand_resume(struct platform_device *plat_dev)
{
#if 0
	if (NORMAL_STANDBY == standby_type) {
		nand_dbg_err("[NAND] nand_resume normal\n");
		NandHwNormalResume();
	} else if (SUPER_STANDBY == standby_type) {
		nand_dbg_err("[NAND] nand_resume super\n");
		NandHwSuperResume();
	}

	nand_dbg_err("[NAND] nand_resume ok\n");
#else
	nand_dbg_err("[NAND] nand_resume\n");
	NandHwSuperResume();
	nand_dbg_err("[NAND] nand_resume ok\n");
#endif
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
static int nand_probe(struct platform_device *plat_dev)
{
    nand_dbg_inf("nand_probe\n");
    return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
static int nand_remove(struct platform_device *plat_dev)
{
	nand_dbg_inf("nand_remove\n");
	return 0;
}

static void nand_release_dev(struct device *dev)
{
	nand_dbg_inf("nand_release dev\n");
	return;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
uint32 shutdown_flush_write_cache(void)
{
	struct _nftl_blk *nftl_blk;
	struct nand_blk_ops *tr = &mytr;

	nftl_blk = tr->nftl_blk_head.nftl_blk_next;

	while (nftl_blk != NULL) {
		nand_dbg_err("shutdown_flush_write_cache\n");
		mutex_lock(nftl_blk->blk_lock);
		nftl_blk->flush_write_cache(nftl_blk, 0xffff);

		print_nftl_zone(nftl_blk->nftl_zone);
		//nand_before_shutdown(nftl_blk->nftl_zone);

		nftl_blk = nftl_blk->nftl_blk_next;

	}
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
void nand_shutdown(struct platform_device *plat_dev)
{
#if 0
	struct nand_blk_dev *dev;
	struct nand_blk_ops *tr = &mytr;

	nand_dbg_err("[NAND]shutdown first\n");
	list_for_each_entry(dev, &tr->devs, list)
	{
		while (blk_fetch_request(dev->rq) != NULL) {
			nand_dbg_err("nand_shutdown wait dev %d\n",
			    dev->devnum);
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(HZ>>3);
		}
	}

	nand_dbg_err("[NAND]shutdown second\n");
	list_for_each_entry(dev, &tr->devs, list)
	{
		while (blk_fetch_request(dev->rq) != NULL) {
			nand_dbg_err("nand_shutdown wait dev %d\n",
			    dev->devnum);
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(HZ>>3);
		}
	}
#endif
	shutdown_flush_write_cache();
	NandHwShutDown();
	nand_dbg_err("[NAND]shutdown end\n");
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
static struct platform_driver nand_driver = {
	.probe = nand_probe,
	.remove = nand_remove,
	.shutdown = nand_shutdown,
	.suspend = nand_suspend,
	.resume = nand_resume,
	.driver = {
		   .name = "sw_nand",
		   .owner = THIS_MODULE,
		   }
};

static struct resource flash_resource = {
	.start = 0,
	.end = 1,
	.flags = 0x1,
};

static struct platform_device nand_device = {
	.name = "sw_nand",
	.id = 33,
	.resource = &flash_resource,
	.num_resources = 1,
	.dev = {
		.release = nand_release_dev,
		}
};

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
int __init nand_init(void)
{
    int ret;
    script_item_u   nand0_used_flag;
    script_item_u   nand_cache_level;
    script_item_u   nand_capacity_level;
    script_item_u   nand_flush_cache_num;
    script_item_value_type_e  type;
    char * dev_name = "nand_dev";
	int storage_type =0;	
	uchar *data = kmalloc(0x400,GFP_KERNEL);

    //get card_line
    type = script_get_item("nand0_para", "nand0_used", &nand0_used_flag);
    if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
    {
        nand_dbg_err("nand type err! %d",type);
    }
    nand_dbg_err("nand init start, nand0_used_flag is %d\n", nand0_used_flag.val);

    nand_cache_level.val = 0;
    type = script_get_item("nand0_para", "nand_cache_level", &nand_cache_level);
    if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
    {
        NAND_Print_DBG("nand_cache_level err! %d",type);
        nand_cache_level.val = 0;
    }

    nand_flush_cache_num.val = 8;
    type = script_get_item("nand0_para", "nand_flush_cache_num", &nand_flush_cache_num);
    if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
    {
        //nand_dbg_err("nand_flush_cache_num err! %d",type);
        nand_flush_cache_num.val = 8;
    }

    nand_capacity_level.val = 0;
    type = script_get_item("nand0_para", "nand_capacity_level", &nand_capacity_level);
    if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
    {
        NAND_Print_DBG("nand_capacity_level err! %d\n",type);
        nand_capacity_level.val = 0;
    }

    //flush_cache_num = nand_flush_cache_num.val;

    //nand_dbg_err("flush_cache_num ! %d",flush_cache_num);

    if(nand0_used_flag.val == 0)
    {
        nand_dbg_err("nand driver is disabled \n");
        return 0;
    }

    spin_lock_init(&nand_int_lock);

    if((nand_wait_rb_mode() != 0) || (nand_wait_dma_mode() != 0))
	{
		NAND_Print_DBG("nand interrupt request\n");
        if (request_irq(SUNXI_IRQ_NAND, nand_interrupt, IRQF_DISABLED, dev_name, &channel0))
        {
            nand_dbg_err("nand interrupte ch0 irqno: %d register error\n", SUNXI_IRQ_NAND);
            return -EAGAIN;
        }
    }


	storage_type = NAND_get_storagetype();

	if ((1 != storage_type)&&(2 != storage_type)) {
		nand_dbg_err("nand init start\n");

		p_nand_info = NandHwInit();
		if (p_nand_info == NULL) {
			kfree(data);
			return -EAGAIN;
		}

	    set_cache_level(p_nand_info,nand_cache_level.val);
	    set_capacity_level(p_nand_info,nand_capacity_level.val);
		ret = nand_info_init(p_nand_info, 0, 8, NULL);
		kfree(data);
		if (ret != 0) {
			nand_dbg_err("nand_info_init error\n");
			return ret;
		}

		if (NAND_CheckBoot() != 0) {
			nand_dbg_err("nand CheckBoot error\n");
		}

		platform_device_register(&nand_device);
	   	platform_driver_register(&nand_driver);

		init_blklayer();
	} else {
		nand_dbg_err
		    ("run nand test for dragonboard\n");
		init_blklayer_for_dragonboard();
		return 0;
	}

	kthread_run(nand_thread, &mytr, "%sd", "nand_rc");

	nand_dbg_err("nand init end\n");
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
void __exit nand_exit(void)
{
    script_item_u   nand0_used_flag;
    script_item_value_type_e  type;

    type = script_get_item("nand0_para", "nand0_used", &nand0_used_flag);
    if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
    nand_dbg_err("nand type err!");
    nand_dbg_err("nand0_used_flag is %d\n", nand0_used_flag.val);

    if(nand0_used_flag.val == 0)
    {
        nand_dbg_err("nand driver is disabled \n");
    }

    platform_driver_unregister(&nand_driver);

	platform_device_unregister(&nand_device);
    exit_blklayer();
#if 0
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&early_suspend);
#endif
#endif
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nand flash groups");
MODULE_DESCRIPTION("Generic NAND flash driver code");
