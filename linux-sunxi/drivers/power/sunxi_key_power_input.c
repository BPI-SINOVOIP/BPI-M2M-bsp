#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/ioport.h>
#include <mach/sys_config.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/clk.h>
#include <mach/sunxi-smc.h>
#include <linux/power/scenelock.h>

#define INPUT_DEV_PWR_NAME          ("sunxi-pwr-key-input")

u32 sun8i_pwr_pin;

struct key_pwrin_data_t {
	u32 dect_pin;
	u32 gpio_irq_num;
	u32 hold_time;
	u32 key_value;
	struct timer_list timer;
};

static struct input_dev *sunxi_pwrin_dev;

static struct key_pwrin_data_t key_pwrin_data = {
	.hold_time = 0,
};

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	/*printk("buttons_irq\n");*/
	mod_timer(&key_pwrin_data.timer, jiffies + HZ / 10);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function(unsigned long data)
{
	u32 pinval;
	static int flag;
	pinval = __gpio_get_value(key_pwrin_data.dect_pin);
	/*printk("val = %d\n", pinval);*/
	if (pinval) {
		if (flag == 0) {
			flag++;
			/*printk("down\n");*/
			input_report_key(sunxi_pwrin_dev, key_pwrin_data.key_value, 1);
			input_sync(sunxi_pwrin_dev);
		}
		mod_timer(&key_pwrin_data.timer, jiffies + HZ / 10);
	} else {
		/* ËÉ¿ª */
		/*printk("up\n");*/
		input_report_key(sunxi_pwrin_dev, key_pwrin_data.key_value, 0);
		input_sync(sunxi_pwrin_dev);
		flag = 0;
	}
}

int key_pwrin_open(struct input_dev *dev)
{
	/*printk("key_pwrin_open\n");*/
	init_timer(&key_pwrin_data.timer);
	key_pwrin_data.timer.function = buttons_timer_function;
	key_pwrin_data.gpio_irq_num = gpio_to_irq(key_pwrin_data.dect_pin);
	request_irq(key_pwrin_data.gpio_irq_num, buttons_irq, IRQF_TRIGGER_RISING,
		    "pwr_key", NULL);
	return 0;
}

void key_pwrin_close(struct input_dev *dev)
{
	/*printk("key_pwrin_close\n");*/
	free_irq(key_pwrin_data.gpio_irq_num, NULL);
	del_timer(&key_pwrin_data.timer);
}

static int sunxi_pwr_key_fetch_sysconfig_para(void)
{
	script_item_u val;
	script_item_value_type_e type;

	type = script_get_item("pwr_ctrl", "pwr_ctrl_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
		printk(KERN_ERR
		       "%s script_parser_fetch \"pwr_ctrl\" pwr_ctrl_used = %d\n",
		       __func__, val.val);
		goto script_get_err;
	}

	if (!val.val) {
		printk(KERN_ERR "%s pwr_ctrl is not used in config\n",
		       __func__);
		goto script_get_err;
	}
	type = script_get_item("pwr_ctrl", "dect_pin", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "get dect_pin failed!\n");
		goto script_get_err;
	} else {
		key_pwrin_data.dect_pin = val.gpio.gpio;
		type = script_get_item("pwr_ctrl", "hold_time", &val);
		if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
			printk(KERN_ERR "get hold_time failed!");
			goto script_get_err;
		} else
			key_pwrin_data.hold_time = val.val;
	}
	type = script_get_item("pwr_ctrl", "pwr_pin", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk(KERN_ERR "get pwr_pin failed!\n");
		goto script_get_err;
	}
	sun8i_pwr_pin = val.gpio.gpio;
	return 0;
script_get_err:
	pr_notice("=========script_get_err============\n");
	return -1;
}

EXPORT_SYMBOL(sun8i_pwr_pin);
static int __init key_pwr_in_init(void)
{
	int ret = 0;
	int err = 0;

	ret = sunxi_pwr_key_fetch_sysconfig_para();
	if (ret)
		return ret;
	sunxi_pwrin_dev = input_allocate_device();
	if (!sunxi_pwrin_dev) {
		pr_err("sunxikbd: not enough memory for input device\n");
		err = -ENOMEM;
		return err;
	}
	sunxi_pwrin_dev->open = key_pwrin_open;
	sunxi_pwrin_dev->close = key_pwrin_close;
	sunxi_pwrin_dev->name = INPUT_DEV_PWR_NAME;
	sunxi_pwrin_dev->phys = "/input0";
	sunxi_pwrin_dev->id.bustype = BUS_HOST;
	sunxi_pwrin_dev->id.vendor = 0x0001;
	sunxi_pwrin_dev->id.product = 0x0001;
	sunxi_pwrin_dev->id.version = 0x0100;
	sunxi_pwrin_dev->evbit[0] = BIT_MASK(EV_KEY);
	key_pwrin_data.key_value = KEY_POWER;
	set_bit(key_pwrin_data.key_value, sunxi_pwrin_dev->keybit);
	err = input_register_device(sunxi_pwrin_dev);
	if (err)
		goto err_out;
	return 0;
err_out:
	input_free_device(sunxi_pwrin_dev);
	return -1;
}

static void __exit key_pwr_in_exit(void)
{
	input_unregister_device(sunxi_pwrin_dev);
	input_free_device(sunxi_pwrin_dev);
}

module_init(key_pwr_in_init);
module_exit(key_pwr_in_exit);
MODULE_AUTHOR("Wang Gang");
MODULE_LICENSE("GPL");
