/*
 * A V4L2 driver for ov7740 cameras.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>


#include "camera.h"


MODULE_AUTHOR("raymonxiu");
MODULE_DESCRIPTION("A low-level driver for ov7740 sensors");
MODULE_LICENSE("GPL");

#define AF_WIN_NEW_COORD
//for internel driver debug
#define DEV_DBG_EN      0
#if(DEV_DBG_EN == 1)
#define vfe_dev_dbg(x,arg...) printk("[OV7740]"x,##arg)
#else
#define vfe_dev_dbg(x,arg...)
#endif
#define vfe_dev_err(x,arg...) printk("[OV7740]"x,##arg)
#define vfe_dev_print(x,arg...) printk("[OV7740]"x,##arg)

#define LOG_ERR_RET(x)  { \
                          int ret;  \
                          ret = x; \
                          if(ret < 0) {\
                            vfe_dev_err("error at %s\n",__func__);  \
                            return ret; \
                          } \
                        }

//define module timing
#define MCLK              (24*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_LOW
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR 0x7740

//define the voltage level of control signal
#define CSI_STBY_ON     1
#define CSI_STBY_OFF    0
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0

#define SENSOR_NAME "ov7740"
#define regval_list reg_list_a8_d8

#define REG_TERM 0xff
#define VAL_TERM 0xff
#define REG_DLY  0xff

//#define AUTO_FPS
#define SHARPNESS 0x10


/*
 * Our nominal (default) frame rate.
 */
#define SENSOR_FRAME_RATE 120

/*
 * The ov7740 sits on i2c with ID 0x42
 */
#define I2C_ADDR 0x42

//static struct delayed_work sensor_s_ae_ratio_work;
static struct v4l2_subdev *glb_sd;

/*
 * Information we maintain about a known sensor.
 */
struct sensor_format_struct;  /* coming later */

struct cfg_array { /* coming later */
	struct regval_list * regs;
	int size;
};

static inline struct sensor_info *to_state(struct v4l2_subdev *sd)
{
  return container_of(sd, struct sensor_info, sd);
}


/*
 * The default register settings
 *
 */

static struct regval_list sensor_default_regs[] = {
};
/*
static struct regval_list sensor_vga_regs[] = { //VGA:  640*480 30fps
	{0x12, 0x80},//
	
	{0xff, 0x80},//delay
	
	{0x13, 0x00},//
	
	{0x11, 0x01},// 0x01 30fps 0x00 60fps
	{0x12, 0x00},//
	{0xd5, 0x10},//
	{0x0c, 0x02},// 0x12:UYUV   0x02:YUYV
	{0x0d, 0x34},//
	{0x17, 0x25},//
	{0x18, 0xa0},//
	{0x19, 0x03},//
	{0x1a, 0xf0},//
	{0x1b, 0x89},//
	{0x22, 0x03},//
	{0x29, 0x17},//
	{0x2b, 0xf8},//
	{0x2c, 0x01},//
	{0x31, 0xa0},//
	{0x32, 0xf0},//
	{0x33, 0xc4},//
	{0x35, 0x05},//
	{0x36, 0x3f},//
	
	{0x04, 0x60},//
	{0x27, 0x80},//
	{0x3d, 0x0f},//
	{0x3e, 0x81},//
	{0x3f, 0x40},//
	{0x40, 0x7f},//
	{0x41, 0x6a},//
	{0x42, 0x29},//
	{0x44, 0xe5},//
	{0x45, 0x41},//
	{0x47, 0x42},//
	{0x48, 0x00},//
	{0x49, 0x61},//
	{0x4a, 0xa1},//
	{0x4b, 0x5e},//
	{0x4c, 0x18},//
	{0x4d, 0x50},//
	{0x4e, 0x13},//
	{0x64, 0x00},//
	{0x67, 0x88},//
	{0x68, 0x1a},//
	
	{0x14, 0x38},//
	{0x24, 0x3c},//
	{0x25, 0x30},//
	{0x26, 0x72},//
	{0x50, 0x97},//
	{0x51, 0x7e},//
	{0x52, 0x00},//
	{0x53, 0x00},//
	{0x20, 0x00},//
	{0x21, 0x23},//
	{0x38, 0x14},//
	{0xe9, 0x00},//
	{0x56, 0x55},//
	{0x57, 0xff},//
	{0x58, 0xff},//
	{0x59, 0xff},//
	{0x5f, 0x04},//
	{0xec, 0x00},//
	{0x13, 0xff},//
	
	{0x80, 0x7d},//
	{0x81, 0x3f},//
	{0x82, 0x32},//
	{0x83, 0x01},//
	{0x38, 0x11},//
	{0x84, 0x70},//
	{0x85, 0x00},//
	{0x86, 0x03},//
	{0x87, 0x01},//
	{0x88, 0x05},//
	{0x89, 0x30},//
	{0x8d, 0x30},//
	{0x8f, 0x85},//
	{0x93, 0x30},//
	{0x95, 0x85},//
	{0x99, 0x30},//
	{0x9b, 0x85},//
	
	{0x9c, 0x08},//
	{0x9d, 0x12},//
	{0x9e, 0x23},//
	{0x9f, 0x45},//
	{0xa0, 0x55},//
	{0xa1, 0x64},//
	{0xa2, 0x72},//
	{0xa3, 0x7f},//
	{0xa4, 0x8b},//
	{0xa5, 0x95},//
	{0xa6, 0xa7},//
	{0xa7, 0xb5},//
	{0xa8, 0xcb},//
	{0xa9, 0xdd},//
	{0xaa, 0xec},//
	{0xab, 0x1a},//
	
	{0xce, 0x78},//
	{0xcf, 0x6e},//
	{0xd0, 0x0a},//
	{0xd1, 0x0c},//
	{0xd2, 0x84},//
	{0xd3, 0x90},//
	{0xd4, 0x1e},//
	
	{0x5a, 0x24},//
	{0x5b, 0x1f},//
	{0x5c, 0x88},//
	{0x5d, 0x60},//
	
	{0xac, 0x6e},//
	{0xbe, 0xff},//
	{0xbf, 0x00},//
	
	//50/60Hz auto detection is XCLK dependent
	//the following is based on XCLK = 24MHz
	{0x70, 0x00},//
	{0x71, 0x34},//
	{0x74, 0x28},//
	{0x75, 0x98},//
	{0x76, 0x00},//
	{0x77, 0x08},//
	{0x78, 0x01},//
	{0x79, 0xc2},//
	{0x7d, 0x02},//
	{0x7a, 0x4e},//
	{0x7b, 0x1f},//
	{0xEC, 0x00},// ;00/80 for manual/auto
	{0x7c, 0x0c},//

#if 0
{0x55 ,0x40},

{0x11 ,0x02},



{0x12 ,0x00},

{0xd5 ,0x10},

{0x0c ,0x12},

{0x0d ,0x34},

{0x17 ,0x25},

{0x18 ,0xa0},

{0x19 ,0x03},

{0x1a ,0xf0},

{0x1b ,0x89},

{0x22 ,0x03},

{0x29 ,0x18},

{0x2b ,0xf8},

{0x2c ,0x01},

{0x31 ,0xa0},

{0x32 ,0xf0},

{0x33 ,0xc4},

{0x35 ,0x05},

{0x36 ,0x3f},

{0x04 ,0x60},

{0x27 ,0x80},

{0x3d ,0x0f},

{0x3e ,0x80},

{0x3f ,0x40},

{0x40 ,0x7f},

{0x41 ,0x6a},

{0x42 ,0x29},

{0x44 ,0x22},

{0x45 ,0x41},

{0x47 ,0x02},

{0x49 ,0x64},

{0x4a ,0xa1},

{0x4b ,0x40},

{0x4c ,0x1a},

{0x4d ,0x50},

{0x4e ,0x13},

{0x64 ,0x00},

{0x67 ,0x88},

{0x68 ,0x1a},



{0x14 ,0x28},

{0x24 ,0x3c},

{0x25 ,0x30},

{0x26 ,0x72},

{0x50 ,0x97},

{0x51 ,0x1f},

{0x52 ,0x00},

{0x53 ,0x00},

{0x20 ,0x00},

{0x21 ,0xcf},

{0x50, 0x4b},

{0x38 ,0x14},

{0xe9 ,0x00},

{0x56 ,0x55},

{0x57 ,0xff},

{0x58 ,0xff},

{0x59 ,0xff},

{0x5f ,0x04},

{0xec ,0x00},

{0x13 ,0xff},



{0x80 ,0x7f},

{0x81 ,0x3f},

{0x82 ,0x32},

{0x83 ,0x01},

{0x38 ,0x11},

{0x84 ,0x70},

{0x85 ,0x00},

{0x86 ,0x03},

{0x87 ,0x01},

{0x88 ,0x05},

{0x89 ,0x30},

{0x8d ,0x30},

{0x8f ,0x85},

{0x93 ,0x30},

{0x95 ,0x85},

{0x99 ,0x30},

{0x9b ,0x85},



{0x9c ,0x08},

{0x9d ,0x12},

{0x9e ,0x23},

{0x9f ,0x45},

{0xa0 ,0x55},

{0xa1 ,0x64},

{0xa2 ,0x72},

{0xa3 ,0x7f},

{0xa4 ,0x8b},

{0xa5 ,0x95},

{0xa6 ,0xa7},

{0xa7 ,0xb5},

{0xa8 ,0xcb},

{0xa9 ,0xdd},

{0xaa ,0xec},

{0xab ,0x1a},



{0xce ,0x78},

{0xcf ,0x6e},

{0xd0 ,0x0a},

{0xd1 ,0x0c},

{0xd2 ,0x84},

{0xd3 ,0x90},

{0xd4 ,0x1e},



{0x5a ,0x24},

{0x5b ,0x1f},

{0x5c ,0x88},

{0x5d ,0x60},



{0xac ,0x6e},

{0xbe ,0xff},

{0xbf ,0x00},



{0x0f ,0x1d},

{0x0f ,0x1f},
#endif
};
*/
static struct regval_list sensor_vga_regs[] = { //VGA:  640*480 60fps
	{0x12, 0x80},//
	
	{0xff, 0x80},//delay
	
	{0x13, 0x00},//
	
	{0x11, 0x00},// 0x01 30fps 0x00 60fps
	{0x12, 0x00},//
	{0xd5, 0x10},//
	{0x0c, 0x02},// 0x12:UYUV   0x02:YUYV
	{0x0d, 0x34},//
	{0x17, 0x25},//
	{0x18, 0xa0},//
	{0x19, 0x03},//
	{0x1a, 0xf0},//
	{0x1b, 0x89},//
	{0x22, 0x03},//
	{0x29, 0x17},//
	{0x2b, 0xf8},//
	{0x2c, 0x01},//
	{0x31, 0xa0},//
	{0x32, 0xf0},//
	{0x33, 0xc4},//
	{0x35, 0x05},//
	{0x36, 0x3f},//
	
	{0x04, 0x60},//
	{0x27, 0x80},//
	{0x3d, 0x08},//
	{0x3e, 0x81},//
	{0x3f, 0x40},//
	{0x40, 0x7f},//
	{0x41, 0x6a},//
	{0x42, 0x29},//
	{0x44, 0xf5},//
	{0x45, 0x41},//
	{0x47, 0x42},//
	{0x48, 0x00},//
	{0x49, 0x61},//
	{0x4a, 0xa1},//
	{0x4b, 0x5e},//
	{0x4c, 0x18},//
	{0x4d, 0x50},//
	{0x4e, 0x13},//
	{0x64, 0x00},//
	{0x67, 0x88},//
	{0x68, 0x1a},//
	
	{0x14, 0x38},//
	{0x24, 0x3c},//
	{0x25, 0x30},//
	{0x26, 0x72},//
	{0x50, 0x2e},//
	{0x51, 0xfc},//
	{0x52, 0x10},//
	{0x53, 0x00},//
	{0x55, 0x40},//
	{0x20, 0x00},//
	{0x21, 0x01},//
	{0x38, 0x14},//
	{0xe9, 0x00},//
	{0x56, 0x55},//
	{0x57, 0xff},//
	{0x58, 0xff},//
	{0x59, 0xff},//
	{0x5f, 0x04},//
	{0xec, 0x00},//
	{0x13, 0xff},//
	
	{0x80, 0x7d},//
	{0x81, 0x3f},//
	{0x82, 0x32},//
	{0x83, 0x01},//
	{0x38, 0x11},//
	{0x84, 0x70},//
	{0x85, 0x00},//
	{0x86, 0x03},//
	{0x87, 0x01},//
	{0x88, 0x05},//
	{0x89, 0x30},//
	{0x8d, 0x30},//
	{0x8f, 0x85},//
	{0x93, 0x30},//
	{0x95, 0x85},//
	{0x99, 0x30},//
	{0x9b, 0x85},//
	
	{0x9c, 0x08},//
	{0x9d, 0x12},//
	{0x9e, 0x23},//
	{0x9f, 0x45},//
	{0xa0, 0x55},//
	{0xa1, 0x64},//
	{0xa2, 0x72},//
	{0xa3, 0x7f},//
	{0xa4, 0x8b},//
	{0xa5, 0x95},//
	{0xa6, 0xa7},//
	{0xa7, 0xb5},//
	{0xa8, 0xcb},//
	{0xa9, 0xdd},//
	{0xaa, 0xec},//
	{0xab, 0x1a},//
	
	{0xce, 0x78},//
	{0xcf, 0x6e},//
	{0xd0, 0x0a},//
	{0xd1, 0x0c},//
	{0xd2, 0x84},//
	{0xd3, 0x90},//
	{0xd4, 0x1e},//
	
	{0x5a, 0x24},//
	{0x5b, 0x1f},//
	{0x5c, 0x88},//
	{0x5d, 0x60},//
	
	{0xac, 0x6e},//
	{0xbe, 0xff},//
	{0xbf, 0x00},//
	
	//50/60Hz auto detection is XCLK dependent
	//the following is based on XCLK = 24MHz
	{0x70, 0x00},//
	{0x71, 0x34},//
	{0x74, 0x28},//
	{0x75, 0x98},//
	{0x76, 0x00},//
	{0x77, 0x08},//
	{0x78, 0x01},//
	{0x79, 0xc2},//
	{0x7d, 0x02},//
	{0x7a, 0x4e},//
	{0x7b, 0x1f},//
	{0xEC, 0x00},// ;00/80 for manual/auto
	{0x7c, 0x0c},//

};

static struct regval_list sensor_qvga_regs[] = { //QVGA:  320*240 
#if 0	//90fps
	{0x12, 0x80},//
	
	{0xff, 0x80},//delay
	
	{0x13, 0x00},//
	
	{0x11, 0x00},// 0x01 30fps 0x00 60fps
	{0x12, 0x00},//
	{0xd5, 0x10},//
	{0x0c, 0x02},// 0x12:UYUV   0x02:YUYV
	{0x0d, 0x34},//
	{0x17, 0x25},//
	{0x18, 0xa0},//
	{0x19, 0x03},//
	{0x1a, 0xf0},//
	{0x1b, 0x89},//
	{0x22, 0x03},//
	{0x29, 0x17},//
	{0x2b, 0xf8},//
	{0x2c, 0x01},//
	{0x31, 0x50},//
	{0x32, 0x78},//
	{0x33, 0xc4},//
	{0x35, 0x05},//
	{0x36, 0x3f},//
	
	{0x04, 0x60},//
	{0x27, 0x80},//
	{0x3d, 0x08},//
	{0x3e, 0x81},//
	{0x3f, 0x40},//
	{0x40, 0x7f},//
	{0x41, 0x6a},//
	{0x42, 0x29},//
	{0x44, 0xf5},//
	{0x45, 0x41},//
	{0x47, 0x42},//
	{0x48, 0x00},//
	{0x49, 0x61},//
	{0x4a, 0xa1},//
	{0x4b, 0x5e},//
	{0x4c, 0x18},//
	{0x4d, 0x50},//
	{0x4e, 0x13},//
	{0x64, 0x00},//
	{0x67, 0x88},//
	{0x68, 0x1a},//
	
	{0x14, 0x38},//
	{0x24, 0x3c},//
	{0x25, 0x30},//
	{0x26, 0x72},//
	{0x50, 0x2e},//
	{0x51, 0xfc},//
	{0x52, 0x10},//
	{0x53, 0x00},//
	{0x55, 0x40},// 90fps
	{0x20, 0x00},//
	{0x21, 0x01},//
	{0x38, 0x14},//
	{0xe9, 0x00},//
	{0x56, 0x55},//
	{0x57, 0xff},//
	{0x58, 0xff},//
	{0x59, 0xff},//
	{0x5f, 0x04},//
	{0xec, 0x00},//
	{0x13, 0xff},//
	
	{0x80, 0x7d},//
	{0x81, 0x3f},//
	{0x82, 0x3f},//
	{0x83, 0x01},//
	{0x38, 0x11},//
	{0x84, 0x70},//
	{0x85, 0x00},//
	{0x86, 0x03},//
	{0x87, 0x01},//
	{0x88, 0x05},//
	{0x89, 0x30},//
	{0x8d, 0x30},//
	{0x8f, 0x85},//
	{0x93, 0x30},//
	{0x95, 0x85},//
	{0x99, 0x30},//
	{0x9b, 0x85},//
	
	{0x9c, 0x08},//
	{0x9d, 0x12},//
	{0x9e, 0x23},//
	{0x9f, 0x45},//
	{0xa0, 0x55},//
	{0xa1, 0x64},//
	{0xa2, 0x72},//
	{0xa3, 0x7f},//
	{0xa4, 0x8b},//
	{0xa5, 0x95},//
	{0xa6, 0xa7},//
	{0xa7, 0xb5},//
	{0xa8, 0xcb},//
	{0xa9, 0xdd},//
	{0xaa, 0xec},//
	{0xab, 0x1a},//
	
	{0xce, 0x78},//
	{0xcf, 0x6e},//
	{0xd0, 0x0a},//
	{0xd1, 0x0c},//
	{0xd2, 0x84},//
	{0xd3, 0x90},//
	{0xd4, 0x1e},//
	
	{0x5a, 0x24},//
	{0x5b, 0x1f},//
	{0x5c, 0x88},//
	{0x5d, 0x60},//
	
	{0xac, 0x6e},//
	{0xbe, 0xff},//
	{0xbf, 0x00},//
	
	//50/60Hz auto detection is XCLK dependent
	//the following is based on XCLK = 24MHz
	{0x70, 0x00},//
	{0x71, 0x34},//
	{0x74, 0x28},//
	{0x75, 0x98},//
	{0x76, 0x00},//
	{0x77, 0x08},//
	{0x78, 0x01},//
	{0x79, 0xc2},//
	{0x7d, 0x02},//
	{0x7a, 0x4e},//
	{0x7b, 0x1f},//
	{0xEC, 0x00},// ;00/80 for manual/auto
	{0x7c, 0x0c},//
#else	//120fps
{0x12, 0x80},//
{0xff, 0x0a},//
{0x13, 0x00},//
{0x11, 0x00},//
{0x12, 0x40},//
{0xd5, 0x10},//
{0x0c, 0x02},//
{0x0d, 0xd2},//
{0x17, 0x25},//
{0x18, 0xa0},//
{0x19, 0x02},//
{0x1a, 0x78},//
{0x1b, 0x89},//
{0x1e, 0x33},//
{0x22, 0x03},//
{0x29, 0x17},//;b4 for vQVGA 100fps
{0x2b, 0xfc},//
{0x2c, 0x00},//
{0x31, 0x50},//
{0x32, 0x78},//
{0x33, 0xc1},//
{0x35, 0x05},//
{0x36, 0x3f},//
{0x04, 0x60},//
{0x27, 0x80},//
{0x3d, 0x08},//
{0x3e, 0x82},//
{0x3f, 0x40},//
{0x40, 0x7f},//
{0x41, 0x6a},//
{0x42, 0x29},//
{0x44, 0xf5},//
{0x45, 0x41},//
{0x47, 0x02},//
{0x49, 0x64},//
{0x4a, 0xa1},//
{0x4b, 0x46},//
{0x4c, 0x18},//
{0x4d, 0x50},//
{0x4e, 0x13},//
{0x64, 0x00},//
{0x67, 0x88},//
{0x68, 0x0a},//
{0x14, 0x38},//
{0x24, 0x3c},//
{0x25, 0x30},//
{0x26, 0x72},//
{0x53, 0x00},//
{0x38, 0x14},//
{0xe9, 0x00},//
{0x56, 0x55},//
{0x57, 0xff},//
{0x58, 0xff},//
{0x59, 0xff},//
{0x5f, 0x04},//
{0x13, 0xcf},//
{0x80, 0x7d},//
{0x81, 0x3f},//
{0x82, 0x3f},//
{0x83, 0x03},//
{0x38, 0x11},//
{0x84, 0x70},//
{0x85, 0x00},//
{0x86, 0x03},//
{0x87, 0x01},//
{0x88, 0x05},//
{0x89, 0x34},//
{0x8d, 0x30},//
{0x8f, 0x85},//
{0x93, 0x30},//
{0x95, 0x85},//
{0x99, 0x30},//
{0x9b, 0x85},//
{0x9c, 0x08},//
{0x9d, 0x12},//
{0x9e, 0x23},//
{0x9f, 0x45},//
{0xa0, 0x55},//
{0xa1, 0x64},//
{0xa2, 0x72},//
{0xa3, 0x7f},//
{0xa4, 0x8b},//
{0xa5, 0x95},//
{0xa6, 0xa7},//
{0xa7, 0xb5},//
{0xa8, 0xcb},//
{0xa9, 0xdd},//
{0xaa, 0xec},//
{0xab, 0x1a},//
{0xce, 0x78},//
{0xcf, 0x6e},//
{0xd0, 0x0a},//
{0xd1, 0x0c},//
{0xd2, 0x84},//
{0xd3, 0x90},//
{0xd4, 0x1e},//
{0x5a, 0x24},//
{0x5b, 0x1f},//
{0x5c, 0x88},//
{0x5d, 0x60},//
{0xac, 0x6e},//
{0xbe, 0xff},//
{0xbf, 0x00},//
{0xec, 0x02},//
#endif
};

/*
 * The white balance settings
 * Here only tune the R G B channel gain.
 * The white balance enalbe bit is modified in sensor_s_autowb and sensor_s_wb
 */
static struct regval_list sensor_wb_manual[] = {
	//null
  //{REG_TERM,VAL_TERM},
};

static struct regval_list sensor_wb_auto_regs[] = {
	{0x13, 0xff}, //AWB on
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},
};

static struct regval_list sensor_wb_incandescence_regs[] = {
	{0x13, 0xff}, //AWB off
	{0x01, 0x96},
	{0x02, 0x40},
	{0x03, 0x4a},
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},

};

static struct regval_list sensor_wb_fluorescent_regs[] = {
	{0x13, 0xff}, //AWB off
	{0x01, 0x84},
	{0x02, 0x4c},
	{0x03, 0x40},
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},
};

static struct regval_list sensor_wb_tungsten_regs[] = {
	{0x13, 0xff}, //AWB off
	{0x01, 0x96},
	{0x02, 0x40},
	{0x03, 0x4a},
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},

};

static struct regval_list sensor_wb_horizon[] = {
//null
};

static struct regval_list sensor_wb_daylight_regs[] = {
	{0x13, 0xff}, //AWB off
	{0x01, 0x5a},
	{0x02, 0x5c},
	{0x03, 0x42},
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},
};

static struct regval_list sensor_wb_flash[] = {
//null
};

static struct regval_list sensor_wb_cloud_regs[] = {
	{0x13, 0xff}, //AWB off
	{0x01, 0x58},
	{0x02, 0x60},
	{0x03, 0x40},
	{0x15, 0x00},
	{0x2d, 0x00},
	{0x2e, 0x00},

};

static struct regval_list sensor_wb_shade[] = {
//null
};

static struct cfg_array sensor_wb[] = {
  {
  	.regs = sensor_wb_manual,             //V4L2_WHITE_BALANCE_MANUAL
    .size = ARRAY_SIZE(sensor_wb_manual),
  },
  {
  	.regs = sensor_wb_auto_regs,          //V4L2_WHITE_BALANCE_AUTO
    .size = ARRAY_SIZE(sensor_wb_auto_regs),
  },
  {
  	.regs = sensor_wb_incandescence_regs, //V4L2_WHITE_BALANCE_INCANDESCENT
    .size = ARRAY_SIZE(sensor_wb_incandescence_regs),
  },
  {
  	.regs = sensor_wb_fluorescent_regs,   //V4L2_WHITE_BALANCE_FLUORESCENT
    .size = ARRAY_SIZE(sensor_wb_fluorescent_regs),
  },
  {
  	.regs = sensor_wb_tungsten_regs,      //V4L2_WHITE_BALANCE_FLUORESCENT_H
    .size = ARRAY_SIZE(sensor_wb_tungsten_regs),
  },
  {
  	.regs = sensor_wb_horizon,            //V4L2_WHITE_BALANCE_HORIZON
    .size = ARRAY_SIZE(sensor_wb_horizon),
  },
  {
  	.regs = sensor_wb_daylight_regs,      //V4L2_WHITE_BALANCE_DAYLIGHT
    .size = ARRAY_SIZE(sensor_wb_daylight_regs),
  },
  {
  	.regs = sensor_wb_flash,              //V4L2_WHITE_BALANCE_FLASH
    .size = ARRAY_SIZE(sensor_wb_flash),
  },
  {
  	.regs = sensor_wb_cloud_regs,         //V4L2_WHITE_BALANCE_CLOUDY
    .size = ARRAY_SIZE(sensor_wb_cloud_regs),
  },
  {
  	.regs = sensor_wb_shade,              //V4L2_WHITE_BALANCE_SHADE
    .size = ARRAY_SIZE(sensor_wb_shade),
  },
//  {
//  	.regs = NULL,
//    .size = 0,
//  },
};


/*
 * The color effect settings
 */
static struct regval_list sensor_colorfx_none_regs[] = {
	{0xDA, 0x00},
	{0xDF, 0x80},
	{0xE0, 0x80},

};

static struct regval_list sensor_colorfx_bw_regs[] = {
	{0xDA, 0x18},
	{0xDF, 0x80},
	{0xE0, 0x80},
};

static struct regval_list sensor_colorfx_sepia_regs[] = {
	//NULL	//{REG_TERM,VAL_TERM},
};

static struct regval_list sensor_colorfx_negative_regs[] = {
	{0xDA, 0x40},
	{0xDF, 0x80},
	{0xE0, 0x80},

};

static struct regval_list sensor_colorfx_emboss_regs[] = {
//NULL  //{REG_TERM,VAL_TERM},
};

static struct regval_list sensor_colorfx_sketch_regs[] = {
	//NULL
  //{REG_TERM,VAL_TERM},
};

static struct regval_list sensor_colorfx_sky_blue_regs[] = {
	{0xDA, 0x18},
	{0xDF, 0xa0},
	{0xE0, 0x40},

};

static struct regval_list sensor_colorfx_grass_green_regs[] = {
	{0xDA, 0x18},
	{0xDF, 0x60},
	{0xE0, 0x60},

};

static struct regval_list sensor_colorfx_skin_whiten_regs[] = {
//NULL
};

static struct regval_list sensor_colorfx_vivid_regs[] = {
//NULL
};

static struct regval_list sensor_colorfx_aqua_regs[] = {
//null
};

static struct regval_list sensor_colorfx_art_freeze_regs[] = {
//null
};

static struct regval_list sensor_colorfx_silhouette_regs[] = {
//null
};

static struct regval_list sensor_colorfx_solarization_regs[] = {
//null
};

static struct regval_list sensor_colorfx_antique_regs[] = {
	{0xDA, 0x18},
	{0xDF, 0x40},
	{0xE0, 0xa0},
};

static struct regval_list sensor_colorfx_set_cbcr_regs[] = {
//null
};

static struct cfg_array sensor_colorfx[] = {
  {
  	.regs = sensor_colorfx_none_regs,         //V4L2_COLORFX_NONE = 0,
    .size = ARRAY_SIZE(sensor_colorfx_none_regs),
  },
  {
  	.regs = sensor_colorfx_bw_regs,           //V4L2_COLORFX_BW   = 1,
    .size = ARRAY_SIZE(sensor_colorfx_bw_regs),
  },
  {
  	.regs = sensor_colorfx_sepia_regs,        //V4L2_COLORFX_SEPIA  = 2,
    .size = ARRAY_SIZE(sensor_colorfx_sepia_regs),
  },
  {
  	.regs = sensor_colorfx_negative_regs,     //V4L2_COLORFX_NEGATIVE = 3,
    .size = ARRAY_SIZE(sensor_colorfx_negative_regs),
  },
  {
  	.regs = sensor_colorfx_emboss_regs,       //V4L2_COLORFX_EMBOSS = 4,
    .size = ARRAY_SIZE(sensor_colorfx_emboss_regs),
  },
  {
  	.regs = sensor_colorfx_sketch_regs,       //V4L2_COLORFX_SKETCH = 5,
    .size = ARRAY_SIZE(sensor_colorfx_sketch_regs),
  },
  {
  	.regs = sensor_colorfx_sky_blue_regs,     //V4L2_COLORFX_SKY_BLUE = 6,
    .size = ARRAY_SIZE(sensor_colorfx_sky_blue_regs),
  },
  {
  	.regs = sensor_colorfx_grass_green_regs,  //V4L2_COLORFX_GRASS_GREEN = 7,
    .size = ARRAY_SIZE(sensor_colorfx_grass_green_regs),
  },
  {
  	.regs = sensor_colorfx_skin_whiten_regs,  //V4L2_COLORFX_SKIN_WHITEN = 8,
    .size = ARRAY_SIZE(sensor_colorfx_skin_whiten_regs),
  },
  {
  	.regs = sensor_colorfx_vivid_regs,        //V4L2_COLORFX_VIVID = 9,
    .size = ARRAY_SIZE(sensor_colorfx_vivid_regs),
  },
  {
  	.regs = sensor_colorfx_aqua_regs,         //V4L2_COLORFX_AQUA = 10,
    .size = ARRAY_SIZE(sensor_colorfx_aqua_regs),
  },
  {
  	.regs = sensor_colorfx_art_freeze_regs,   //V4L2_COLORFX_ART_FREEZE = 11,
    .size = ARRAY_SIZE(sensor_colorfx_art_freeze_regs),
  },
  {
  	.regs = sensor_colorfx_silhouette_regs,   //V4L2_COLORFX_SILHOUETTE = 12,
    .size = ARRAY_SIZE(sensor_colorfx_silhouette_regs),
  },
  {
  	.regs = sensor_colorfx_solarization_regs, //V4L2_COLORFX_SOLARIZATION = 13,
    .size = ARRAY_SIZE(sensor_colorfx_solarization_regs),
  },
  {
  	.regs = sensor_colorfx_antique_regs,      //V4L2_COLORFX_ANTIQUE = 14,
    .size = ARRAY_SIZE(sensor_colorfx_antique_regs),
  },
  {
  	.regs = sensor_colorfx_set_cbcr_regs,     //V4L2_COLORFX_SET_CBCR = 15,
    .size = ARRAY_SIZE(sensor_colorfx_set_cbcr_regs),
  },
};



/*
 * The brightness setttings
 */
static struct regval_list sensor_brightness_neg4_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x40},
};

static struct regval_list sensor_brightness_neg3_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x30},
};

static struct regval_list sensor_brightness_neg2_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x20},
};

static struct regval_list sensor_brightness_neg1_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x10},
};

static struct regval_list sensor_brightness_zero_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x30},
};

static struct regval_list sensor_brightness_pos1_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x10},
};

static struct regval_list sensor_brightness_pos2_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x20},
};

static struct regval_list sensor_brightness_pos3_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x30},
};

static struct regval_list sensor_brightness_pos4_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x40},
};

static struct cfg_array sensor_brightness[] = {
  {
  	.regs = sensor_brightness_neg4_regs,
  	.size = ARRAY_SIZE(sensor_brightness_neg4_regs),
  },
  {
  	.regs = sensor_brightness_neg3_regs,
  	.size = ARRAY_SIZE(sensor_brightness_neg3_regs),
  },
  {
  	.regs = sensor_brightness_neg2_regs,
  	.size = ARRAY_SIZE(sensor_brightness_neg2_regs),
  },
  {
  	.regs = sensor_brightness_neg1_regs,
  	.size = ARRAY_SIZE(sensor_brightness_neg1_regs),
  },
  {
  	.regs = sensor_brightness_zero_regs,
  	.size = ARRAY_SIZE(sensor_brightness_zero_regs),
  },
  {
  	.regs = sensor_brightness_pos1_regs,
  	.size = ARRAY_SIZE(sensor_brightness_pos1_regs),
  },
  {
  	.regs = sensor_brightness_pos2_regs,
  	.size = ARRAY_SIZE(sensor_brightness_pos2_regs),
  },
  {
  	.regs = sensor_brightness_pos3_regs,
  	.size = ARRAY_SIZE(sensor_brightness_pos3_regs),
  },
  {
  	.regs = sensor_brightness_pos4_regs,
  	.size = ARRAY_SIZE(sensor_brightness_pos4_regs),
  },
};

/*
 * The contrast setttings
 */
static struct regval_list sensor_contrast_neg4_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x10},
	{0xE3, 0x20},
	{0xE4, 0x06},
};

static struct regval_list sensor_contrast_neg3_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x14},
	{0xE3, 0x20},
	{0xE4, 0x06},
};

static struct regval_list sensor_contrast_neg2_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x18},
	{0xE3, 0x20},
	{0xE4, 0x06},
};

static struct regval_list sensor_contrast_neg1_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x1C},
	{0xE3, 0x20},
	{0xE4, 0x06},
};

static struct regval_list sensor_contrast_zero_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x20},
	{0xE3, 0x00},
	{0xE4, 0x02},
};

static struct regval_list sensor_contrast_pos1_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x24},
	{0xE3, 0x00},
	{0xE4, 0x02},
};

static struct regval_list sensor_contrast_pos2_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x28},
	{0xE3, 0x00},
	{0xE4, 0x02},
};

static struct regval_list sensor_contrast_pos3_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x2C},
	{0xE3, 0x00},
	{0xE4, 0x02},
};

static struct regval_list sensor_contrast_pos4_regs[] = {
	{0xDA, 0x04},
	{0xE1, 0x20},
	{0xE2, 0x30},
	{0xE3, 0x00},
	{0xE4, 0x02},
};

static struct cfg_array sensor_contrast[] = {
  {
  	.regs = sensor_contrast_neg4_regs,
  	.size = ARRAY_SIZE(sensor_contrast_neg4_regs),
  },
  {
  	.regs = sensor_contrast_neg3_regs,
  	.size = ARRAY_SIZE(sensor_contrast_neg3_regs),
  },
  {
  	.regs = sensor_contrast_neg2_regs,
  	.size = ARRAY_SIZE(sensor_contrast_neg2_regs),
  },
  {
  	.regs = sensor_contrast_neg1_regs,
  	.size = ARRAY_SIZE(sensor_contrast_neg1_regs),
  },
  {
  	.regs = sensor_contrast_zero_regs,
  	.size = ARRAY_SIZE(sensor_contrast_zero_regs),
  },
  {
  	.regs = sensor_contrast_pos1_regs,
  	.size = ARRAY_SIZE(sensor_contrast_pos1_regs),
  },
  {
  	.regs = sensor_contrast_pos2_regs,
  	.size = ARRAY_SIZE(sensor_contrast_pos2_regs),
  },
  {
  	.regs = sensor_contrast_pos3_regs,
  	.size = ARRAY_SIZE(sensor_contrast_pos3_regs),
  },
  {
  	.regs = sensor_contrast_pos4_regs,
  	.size = ARRAY_SIZE(sensor_contrast_pos4_regs),
  },
};

/*
 * The saturation setttings
 */
static struct regval_list sensor_saturation_neg4_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x00},
	{0xDE, 0x00},
};

static struct regval_list sensor_saturation_neg3_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x10},
	{0xDE, 0x10},
};

static struct regval_list sensor_saturation_neg2_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x20},
	{0xDE, 0x20},
};

static struct regval_list sensor_saturation_neg1_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x30},
	{0xDE, 0x30},
};

static struct regval_list sensor_saturation_zero_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x40},
	{0xDE, 0x40},
};

static struct regval_list sensor_saturation_pos1_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x50},
	{0xDE, 0x50},
};

static struct regval_list sensor_saturation_pos2_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x60},
	{0xDE, 0x60},
};

static struct regval_list sensor_saturation_pos3_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x70},
	{0xDE, 0x70},
};

static struct regval_list sensor_saturation_pos4_regs[] = {
	{0xDA, 0x02},
	{0xDD, 0x80},
	{0xDE, 0x80},
};

static struct cfg_array sensor_saturation[] = {
  {
  	.regs = sensor_saturation_neg4_regs,
  	.size = ARRAY_SIZE(sensor_saturation_neg4_regs),
  },
  {
  	.regs = sensor_saturation_neg3_regs,
  	.size = ARRAY_SIZE(sensor_saturation_neg3_regs),
  },
  {
  	.regs = sensor_saturation_neg2_regs,
  	.size = ARRAY_SIZE(sensor_saturation_neg2_regs),
  },
  {
  	.regs = sensor_saturation_neg1_regs,
  	.size = ARRAY_SIZE(sensor_saturation_neg1_regs),
  },
  {
  	.regs = sensor_saturation_zero_regs,
  	.size = ARRAY_SIZE(sensor_saturation_zero_regs),
  },
  {
  	.regs = sensor_saturation_pos1_regs,
  	.size = ARRAY_SIZE(sensor_saturation_pos1_regs),
  },
  {
  	.regs = sensor_saturation_pos2_regs,
  	.size = ARRAY_SIZE(sensor_saturation_pos2_regs),
  },
  {
  	.regs = sensor_saturation_pos3_regs,
  	.size = ARRAY_SIZE(sensor_saturation_pos3_regs),
  },
  {
  	.regs = sensor_saturation_pos4_regs,
  	.size = ARRAY_SIZE(sensor_saturation_pos4_regs),
  },
};

/*
 * The exposure target setttings
 */
static struct regval_list sensor_ev_neg4_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x40},
};

static struct regval_list sensor_ev_neg3_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x30},
};

static struct regval_list sensor_ev_neg2_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x20},
};

static struct regval_list sensor_ev_neg1_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x06},
	{0xE3, 0x10},
};

static struct regval_list sensor_ev_zero_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x30},
};

static struct regval_list sensor_ev_pos1_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x10},
};

static struct regval_list sensor_ev_pos2_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x20},
};

static struct regval_list sensor_ev_pos3_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x30},
};

static struct regval_list sensor_ev_pos4_regs[] = {
	{0xDA, 0x04},
	{0xE4, 0x0E},
	{0xE3, 0x40},
};

static struct cfg_array sensor_ev[] = {
  {
  	.regs = sensor_ev_neg4_regs,
  	.size = ARRAY_SIZE(sensor_ev_neg4_regs),
  },
  {
  	.regs = sensor_ev_neg3_regs,
  	.size = ARRAY_SIZE(sensor_ev_neg3_regs),
  },
  {
  	.regs = sensor_ev_neg2_regs,
  	.size = ARRAY_SIZE(sensor_ev_neg2_regs),
  },
  {
  	.regs = sensor_ev_neg1_regs,
  	.size = ARRAY_SIZE(sensor_ev_neg1_regs),
  },
  {
  	.regs = sensor_ev_zero_regs,
  	.size = ARRAY_SIZE(sensor_ev_zero_regs),
  },
  {
  	.regs = sensor_ev_pos1_regs,
  	.size = ARRAY_SIZE(sensor_ev_pos1_regs),
  },
  {
  	.regs = sensor_ev_pos2_regs,
  	.size = ARRAY_SIZE(sensor_ev_pos2_regs),
  },
  {
  	.regs = sensor_ev_pos3_regs,
  	.size = ARRAY_SIZE(sensor_ev_pos3_regs),
  },
  {
  	.regs = sensor_ev_pos4_regs,
  	.size = ARRAY_SIZE(sensor_ev_pos4_regs),
  },
};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */


static struct regval_list sensor_fmt_yuv422_yuyv[] = {
	{0x0c,0x02},
	{0xd9,0x00},
};

static struct regval_list sensor_fmt_yuv422_yvyu[] = {
	{0x0c,0x02},
	{0xd9,0x20},
};

static struct regval_list sensor_fmt_yuv422_vyuy[] = {
	{0x0c,0x12},
	{0xd9,0x20},
};

static struct regval_list sensor_fmt_yuv422_uyvy[] = {
	{0x0c,0x12},
	{0xd9,0x00},
};


/*
 * Low-level register I/O.
 *
 */


/*
 * On most platforms, we'd rather do straight i2c I/O.
 */
static int sensor_read(struct v4l2_subdev *sd, unsigned short reg,
    unsigned char *value)
{
	int ret=0;
	int cnt=0;

  ret = cci_read_a8_d8(sd,reg,value);
  while(ret!=0&&cnt<2)
  {
  	ret = cci_read_a8_d8(sd,reg,value);
  	cnt++;
  }
  if(cnt>0)
  	vfe_dev_dbg("sensor read retry=%d\n",cnt);

  return ret;
}

/*static int reg_val_show(struct v4l2_subdev *sd,unsigned char reg)
{
	unsigned char tmp;
	sensor_read(sd,reg,&tmp);
	printk("0x%x value is 0x%x\n",reg,tmp);
	return 0;
}*/

static int sensor_write(struct v4l2_subdev *sd, unsigned short reg,
    unsigned char value)
{
	int ret=0;
	int cnt=0;
  ret = cci_write_a8_d8(sd,reg,value);
  while(ret!=0&&cnt<2)
  {
  	ret = cci_write_a8_d8(sd,reg,value);
  	cnt++;
  }
  if(cnt>0)
  	vfe_dev_dbg("sensor write retry=%d\n",cnt);

  //reg_val_show(sd,reg);

  return ret;
}

/*
 * Write a list of register settings;
 */
static int sensor_write_array(struct v4l2_subdev *sd, struct regval_list *regs, int array_size)
{
	int i=0;

  if(!regs)
  	return -EINVAL;

  while(i<array_size)
  {
    if(regs->addr == REG_DLY) {
      msleep(regs->data);
    }
    else {
    	//printk("write 0x%x=0x%x\n", regs->addr, regs->data);
      LOG_ERR_RET(sensor_write(sd, regs->addr, regs->data))
    }
    i++;
    regs++;
  }
  return 0;
}


/*
 * Write a list of continuous register setting;
 */
 /*
static int sensor_write_continuous(struct v4l2_subdev *sd, unsigned short addr, unsigned char vals[] , uint size)
{
  struct i2c_client *client = v4l2_get_subdevdata(sd);
  struct i2c_msg msg;
  unsigned char data[2+32];
  unsigned char *p = vals;
  int ret,i;

  while (size > 0) {
    int len = size > 32 ? 32 : size;
    data[0] = (addr&0xff00) >> 8;
    data[1] = (addr&0x00ff);

    for(i = 2; i < 2+len; i++)
      data[i] = *p++;

    msg.addr = client->addr;
    msg.flags = 0;
    msg.len = 2+len;
    msg.buf = data;

    ret = i2c_transfer(client->adapter, &msg, 1);

    if (ret > 0) {
      ret = 0;
    } else if (ret < 0) {
      vfe_dev_err("sensor_write error!\n");
    }
    addr += len;
    size -= len;
  }
  return ret;
}
*/
/*

static unsigned char current_lum=0xff;
static unsigned char sensor_get_lum(struct v4l2_subdev *sd)
{
  sensor_read(sd, 0x56a1, &current_lum);
  vfe_dev_cap_dbg("check luminance=0x%x\n",current_lum);
  return current_lum;
}
*/
/* stuff about exposure when capturing image and video*/
//static int sensor_s_denoise_value(struct v4l2_subdev *sd, unsigned char value);
//unsigned char ogain,oexposurelow,oexposuremid,oexposurehigh;
//unsigned int preview_exp_line,preview_fps;
//unsigned long preview_pclk;

#ifdef _FLASH_FUNC_
void check_to_flash(struct v4l2_subdev *sd)
{
  struct sensor_info *info = to_state(sd);
  if(info->flash_mode==V4L2_FLASH_LED_MODE_FLASH)
  {
    to_flash=1;
  }
  else if(info->flash_mode==V4L2_FLASH_LED_MODE_AUTO)
  {
    sensor_get_lum(sd);
    if( current_lum<flash_auto_level )
      to_flash=1;
    else
      to_flash=0;
  }
  else
  {
    to_flash=0;
  }

  vfe_dev_dbg("to_flash=%d\n",to_flash);
}
#endif



/* *********************************************begin of ******************************************** */

static int sensor_g_hflip(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;

  LOG_ERR_RET(sensor_read(sd, 0x0C, &rdval))

  rdval &= (1<<6);
  rdval >>= 6;

  *value = rdval;

  info->hflip = *value;
  return 0;
}

static int sensor_s_hflip(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;

  LOG_ERR_RET(sensor_read(sd, 0x0C, &rdval))

  if(info->hflip == value)
    return 0;

  switch (value) {
    case 0:
      rdval &= 0xbf;
      break;
    case 1:
      rdval |= 0x40;
      break;
    default:
      return -EINVAL;
  }

  LOG_ERR_RET(sensor_write(sd, 0x0C, rdval))

  usleep_range(10000,12000);
  info->hflip = value;
  return 0;
}

static int sensor_g_vflip(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;

  LOG_ERR_RET(sensor_read(sd, 0x0C, &rdval))

  rdval &= (1<<7);
  *value = rdval;
  rdval >>= 7;

  info->vflip = *value;
  return 0;
}

static int sensor_s_vflip(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;

  if(info->vflip == value)
    return 0;

  LOG_ERR_RET(sensor_read(sd, 0x0C, &rdval))

  switch (value) {
    case 0:
      rdval &= 0x7f;
      break;
    case 1:
      rdval |= 0x80;
      break;
    default:
      return -EINVAL;
  }

  LOG_ERR_RET(sensor_write(sd, 0x0C, rdval))

  usleep_range(10000,12000);
  info->vflip = value;
  return 0;
}

static int sensor_g_autogain(struct v4l2_subdev *sd, __s32 *value)
{
    return -EINVAL;
}

static int sensor_s_autogain(struct v4l2_subdev *sd, int value)
{
    return -EINVAL;
}

static int sensor_g_autoexp(struct v4l2_subdev *sd, __s32 *value)
{
	int ret;
	struct sensor_info *info = to_state(sd);
	unsigned char val;

	ret = sensor_read(sd, 0x13, &val);
	if (ret < 0) {
		vfe_dev_err("sensor_read err at sensor_g_autoexp!\n");
		return ret;
	}

	val &= 0x01;
	if (val == 0x01) {
		*value = V4L2_EXPOSURE_AUTO;
	}
	else
	{
		*value = V4L2_EXPOSURE_MANUAL;
	}

	info->autoexp = *value;
	return 0;
}

static int sensor_s_autoexp(struct v4l2_subdev *sd,
    enum v4l2_exposure_auto_type value)
{
	int ret;
	struct sensor_info *info = to_state(sd);
	//struct regval_list regs;
	unsigned char val;

	//regs.reg_num[0] = 0x13;
	ret = sensor_read(sd, 0x13, &val);
	if (ret < 0) {
		vfe_dev_err("sensor_read err at sensor_s_autoexp!\n");
		return ret;
	}

	switch (value) {
		case V4L2_EXPOSURE_AUTO:
		  val |= 0x01;
			break;
		case V4L2_EXPOSURE_MANUAL:
			val &= 0xfe;
			break;
		case V4L2_EXPOSURE_SHUTTER_PRIORITY:
			return -EINVAL;
		case V4L2_EXPOSURE_APERTURE_PRIORITY:
			return -EINVAL;
		default:
			return -EINVAL;
	}

	ret = sensor_write(sd, 0x13, val);
	if (ret < 0) {
		vfe_dev_err("sensor_write err at sensor_s_autoexp!\n");
		return ret;
	}

	msleep(50);

	info->autoexp = value;
	return 0;
}

static int sensor_g_autowb(struct v4l2_subdev *sd, int *value)
{
	int ret;
	struct sensor_info *info = to_state(sd);
	//struct regval_list regs;
	unsigned char val;

	//regs.reg_num[0] = 0x13;
	ret = sensor_read(sd, 0x80, &val);
	if (ret < 0) {
		vfe_dev_err("sensor_read err at sensor_g_autowb!\n");
		return ret;
	}

	val &= (1<<4);
	val = val>>4;		//0x80 bit4 is awb enable

	*value = val;
	info->autowb = *value;

	return 0;
}

static int sensor_s_autowb(struct v4l2_subdev *sd, int value)
{
	int ret;
	struct sensor_info *info = to_state(sd);
	unsigned char val;

	ret = sensor_write_array(sd, sensor_wb_auto_regs, ARRAY_SIZE(sensor_wb_auto_regs));
	if (ret < 0) {
		vfe_dev_err("sensor_write_array err at sensor_s_autowb!\n");
		return ret;
	}

	ret = sensor_read(sd, 0x80, &val);
	if (ret < 0) {
		vfe_dev_err("sensor_read err at sensor_s_autowb!\n");
		return ret;
	}

	switch(value) {
	case 0:
		val |= 0x40;
		break;
	case 1:
		val &=~0x40;
		break;
	default:
		break;
	}
	ret = sensor_write(sd, 0x80, val);
	if (ret < 0) {
		vfe_dev_err("sensor_write err at sensor_s_autowb!\n");
		return ret;
	}

	msleep(50);

	info->autowb = value;
	return 0;
}

static int sensor_g_hue(struct v4l2_subdev *sd, __s32 *value)
{
  return -EINVAL;
}

static int sensor_s_hue(struct v4l2_subdev *sd, int value)
{
  return -EINVAL;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
  return -EINVAL;
}

static int sensor_s_gain(struct v4l2_subdev *sd, int value)
{
  return -EINVAL;
}

/* *********************************************end of ******************************************** */

static int sensor_g_brightness(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);

  *value = info->brightness;
  return 0;
}

static int sensor_s_brightness(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);

  if(info->brightness == value)
    return 0;

  if(value < -4 || value > 4)
    return -ERANGE;

  LOG_ERR_RET(sensor_write_array(sd, sensor_brightness[value+4].regs, sensor_brightness[value+4].size))

  info->brightness = value;
  return 0;
}

static int sensor_g_contrast(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);

  *value = info->contrast;
  return 0;
}

static int sensor_s_contrast(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);

  if(info->contrast == value)
    return 0;

  if(value < -4 || value > 4)
    return -ERANGE;

  LOG_ERR_RET(sensor_write_array(sd, sensor_contrast[value+4].regs, sensor_contrast[value+4].size))

  info->contrast = value;
  return 0;
}

static int sensor_g_saturation(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);

  *value = info->saturation;
  return 0;
}

static int sensor_s_saturation(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);

  if(info->saturation == value)
    return 0;

  if(value < -4 || value > 4)
    return -ERANGE;

  LOG_ERR_RET(sensor_write_array(sd, sensor_saturation[value+4].regs, sensor_saturation[value+4].size))

  info->saturation = value;
  return 0;
}

static int sensor_g_exp_bias(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);

  *value = info->exp_bias;
  return 0;
}

static int sensor_s_exp_bias(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);

  if(info->exp_bias == value)
    return 0;

  if(value < -4 || value > 4)
    return -ERANGE;
      /*
    sensor_write(sd, 0x3503, 0x07);
    sensor_get_preview_exposure(sd);
    sensor_write(sd, 0x3503, 0x00);
	*/
//  if(value>=1)
//  {
//    night_mode=value;
//    //Nfrms=value+1;
//    cap_manual_gain=value*0x10;
//    printk("night_mode=%d Nfrms=%d\n",night_mode, Nfrms);
//    //sensor_write(sd, 0x5690, 0x01);
//  }
//  else
//  {
//    unsigned char lum;
//    night_mode=0;
//    //Nfrms=1;
//    sensor_get_lum(sd);
//    sensor_get_preview_exposure(sd);
//    //sensor_write(sd, 0x5690, 0x00);
//  }

  LOG_ERR_RET(sensor_write_array(sd, sensor_ev[value+4].regs, sensor_ev[value+4].size))

  info->exp_bias = value;
  return 0;
}

static int sensor_g_wb(struct v4l2_subdev *sd, int *value)
{
  struct sensor_info *info = to_state(sd);
  enum v4l2_auto_n_preset_white_balance *wb_type = (enum v4l2_auto_n_preset_white_balance*)value;

  *wb_type = info->wb;

  return 0;
}

static int sensor_s_wb(struct v4l2_subdev *sd,
    enum v4l2_auto_n_preset_white_balance value)
{
  struct sensor_info *info = to_state(sd);

  if(info->capture_mode == V4L2_MODE_IMAGE)
    return 0;

  if(info->wb == value)
    return 0;

  LOG_ERR_RET(sensor_write_array(sd, sensor_wb[value].regs ,sensor_wb[value].size) )

  if (value == V4L2_WHITE_BALANCE_AUTO)
    info->autowb = 1;
  else
    info->autowb = 0;

  info->wb = value;
  return 0;
}

static int sensor_g_colorfx(struct v4l2_subdev *sd,
    __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  enum v4l2_colorfx *clrfx_type = (enum v4l2_colorfx*)value;

  *clrfx_type = info->clrfx;
  return 0;
}

static int sensor_s_colorfx(struct v4l2_subdev *sd,
    enum v4l2_colorfx value)
{
  struct sensor_info *info = to_state(sd);

  if(info->clrfx == value)
    return 0;

  LOG_ERR_RET(sensor_write_array(sd, sensor_colorfx[value].regs, sensor_colorfx[value].size))

  info->clrfx = value;
  return 0;
}

static int sensor_g_flash_mode(struct v4l2_subdev *sd,
    __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  enum v4l2_flash_led_mode *flash_mode = (enum v4l2_flash_led_mode*)value;

  *flash_mode = info->flash_mode;
  return 0;
}

static int sensor_s_flash_mode(struct v4l2_subdev *sd,
    enum v4l2_flash_led_mode value)
{
	struct sensor_info *info = to_state(sd);
/*	struct vfe_dev *dev=(struct vfe_dev *)dev_get_drvdata(sd->v4l2_dev->dev);
	int flash_on,flash_off;

	flash_on = (dev->flash_pol!=0)?1:0;
	flash_off = (flash_on==1)?0:1;

	switch (value) {
	case V4L2_FLASH_MODE_OFF:
		csi_gpio_write(sd,&dev->flash_io,flash_off);
		break;
	case V4L2_FLASH_MODE_AUTO:
		return -EINVAL;
		break;
	case V4L2_FLASH_MODE_ON:
		csi_gpio_write(sd,&dev->flash_io,flash_on);
		break;
	case V4L2_FLASH_MODE_TORCH:
		return -EINVAL;
		break;
	case V4L2_FLASH_MODE_RED_EYE:
		return -EINVAL;
		break;
	default:
		return -EINVAL;
	}
	*/
	info->flash_mode = value;
	return 0;
}

/*
 * Stuff that knows about the sensor.
 */

static int sensor_power(struct v4l2_subdev *sd, int on)
{
  int ret;
  //insure that clk_disable() and clk_enable() are called in pair
  //when calling CSI_SUBDEV_STBY_ON/OFF and CSI_SUBDEV_PWR_ON/OFF
  ret = 0;
  switch(on)
  {
    case CSI_SUBDEV_STBY_ON:
      vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
      #ifdef _FLASH_FUNC_
      io_set_flash_ctrl(sd, SW_CTRL_FLASH_OFF, to_state(sd)->fl_dev_info);
      #endif
  //    sensor_s_release_af(sd);
      //software standby
  //    ret = sensor_write_array(sd, sensor_sw_stby_on_regs ,ARRAY_SIZE(sensor_sw_stby_on_regs));
  //    if(ret < 0)
   //     vfe_dev_err("soft stby falied!\n");
      usleep_range(10000,12000);
      //disable io oe
      vfe_dev_print("disalbe oe!\n");
  //    ret = sensor_write_array(sd, sensor_oe_disable_regs, ARRAY_SIZE(sensor_oe_disable_regs));
 //     if(ret < 0)
  //      vfe_dev_err("disalbe oe falied!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);
      //standby on io
      vfe_gpio_write(sd,PWDN,CSI_STBY_ON);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);
      //inactive mclk after stadby in
      vfe_set_mclk(sd,OFF);
      break;
    case CSI_SUBDEV_STBY_OFF:
      vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);
      //active mclk before stadby out
      vfe_set_mclk_freq(sd,MCLK);
      vfe_set_mclk(sd,ON);
      usleep_range(10000,12000);
      //standby off io
      vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
      usleep_range(10000,12000);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);
      vfe_dev_print("enable oe!\n");
//      ret = sensor_write_array(sd, sensor_oe_enable_regs,  ARRAY_SIZE(sensor_oe_enable_regs));
//      if(ret < 0)
//        vfe_dev_err("enable oe falied!\n");
      //software standby
 //     ret = sensor_write_array(sd, sensor_sw_stby_off_regs ,ARRAY_SIZE(sensor_sw_stby_off_regs));
  //    if(ret < 0)
//vfe_dev_err("soft stby off falied!\n");
      usleep_range(10000,12000);
      break;
    case CSI_SUBDEV_PWR_ON:
      vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);
      //power on reset
      vfe_gpio_set_status(sd,PWDN,1);//set the gpio to output
      vfe_gpio_set_status(sd,RESET,1);//set the gpio to output
      //power down io
      vfe_gpio_write(sd,PWDN,CSI_STBY_ON);
      //reset on io
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      usleep_range(1000,1200);
      //active mclk before power on
      vfe_set_mclk_freq(sd,MCLK);
      vfe_set_mclk(sd,ON);
      usleep_range(10000,12000);
      //power supply
      vfe_gpio_write(sd,POWER_EN,CSI_PWR_ON);
      vfe_set_pmu_channel(sd,IOVDD,ON);
      vfe_set_pmu_channel(sd,AVDD,ON);
      vfe_set_pmu_channel(sd,DVDD,ON);
      vfe_set_pmu_channel(sd,AFVDD,ON);
      //standby off io
      vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
      usleep_range(10000,12000);
      //reset after power on
      vfe_gpio_write(sd,RESET,CSI_RST_OFF);
      usleep_range(30000,31000);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);
      break;
    case CSI_SUBDEV_PWR_OFF:
      vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);
      //inactive mclk before power off
      vfe_set_mclk(sd,OFF);
      //power supply off
      vfe_gpio_write(sd,POWER_EN,CSI_PWR_OFF);
      vfe_set_pmu_channel(sd,AFVDD,OFF);
      vfe_set_pmu_channel(sd,DVDD,OFF);
      vfe_set_pmu_channel(sd,AVDD,OFF);
      vfe_set_pmu_channel(sd,IOVDD,OFF);
      //standby and reset io
      usleep_range(10000,12000);
      vfe_gpio_write(sd,POWER_EN,CSI_STBY_OFF);
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      //set the io to hi-z
      vfe_gpio_set_status(sd,RESET,0);//set the gpio to input
      vfe_gpio_set_status(sd,PWDN,0);//set the gpio to input
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);
      break;
    default:
      return -EINVAL;
  }

  return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
  switch(val)
  {
    case 0:
      vfe_gpio_write(sd,RESET,CSI_RST_OFF);
      usleep_range(10000,12000);
      break;
    case 1:
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      usleep_range(10000,12000);
      break;
    default:
      return -EINVAL;
  }

  return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
  unsigned char rdval;
  LOG_ERR_RET(sensor_write(sd, 0xff, 0x01))
  LOG_ERR_RET(sensor_read(sd, 0x0a, &rdval))

  if(rdval != 0x77)
    return -ENODEV;

  LOG_ERR_RET(sensor_read(sd, 0x0b, &rdval))
  printk("%s,%d:rdval = 0x%x\n",__func__,__LINE__,rdval);
  if(rdval != 0x42)
    return -ENODEV;

  return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
  int ret;
  struct sensor_info *info = to_state(sd);
  #ifdef _FLASH_FUNC_
  struct vfe_dev *dev=(struct vfe_dev *)dev_get_drvdata(sd->v4l2_dev->dev);
  #endif

  vfe_dev_dbg("sensor_init 0x%x\n",val);

  /*Make sure it is a target sensor*/
  ret = sensor_detect(sd);
  if (ret) {
    vfe_dev_err("chip found is not an target chip.\n");
    return ret;
  }

  vfe_get_standby_mode(sd,&info->stby_mode);

  if((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY) \
      && info->init_first_flag == 0) {
    vfe_dev_print("stby_mode and init_first_flag = 0\n");
    return 0;
  }

  info->focus_status = 0;
  info->low_speed = 0;
  info->width = 0;
  info->height = 0;
  info->brightness = 0;
  info->contrast = 0;
  info->saturation = 0;
  info->hue = 0;
  info->hflip = 0;
  info->vflip = 0;
  info->gain = 0;
  info->autogain = 1;
  info->exp_bias = 0;
  info->autoexp = 1;
  info->autowb = 1;
  info->wb = V4L2_WHITE_BALANCE_AUTO;
  info->clrfx = V4L2_COLORFX_NONE;
  info->band_filter = V4L2_CID_POWER_LINE_FREQUENCY_50HZ;

//  info->af_ctrl = V4L2_AF_RELEASE;
  info->tpf.numerator = 1;
  info->tpf.denominator = 30;    /* 30fps */

  ret = sensor_write_array(sd, sensor_default_regs, ARRAY_SIZE(sensor_default_regs));
  if(ret < 0) {
    vfe_dev_err("write sensor_default_regs error\n");
    return ret;
  }

//  sensor_s_band_filter(sd, V4L2_CID_POWER_LINE_FREQUENCY_50HZ);

  if(info->stby_mode == 0)
    info->init_first_flag = 0;

  info->preview_first_flag = 1;
//  INIT_DELAYED_WORK(&sensor_s_ae_ratio_work, sensor_s_ae_ratio);

  #ifdef _FLASH_FUNC_
  if(dev->flash_used==1)
  {
    info->fl_dev_info=&fl_info;
    info->fl_dev_info->dev_if=0;
    info->fl_dev_info->en_pol=FLASH_EN_POL;
    info->fl_dev_info->fl_mode_pol=FLASH_MODE_POL;
    info->fl_dev_info->light_src=0x01;
    info->fl_dev_info->flash_intensity=400;
    info->fl_dev_info->flash_level=0x01;
    info->fl_dev_info->torch_intensity=200;
    info->fl_dev_info->torch_level=0x01;
    info->fl_dev_info->timeout_counter=300*1000;
    config_flash_mode(sd, V4L2_FLASH_LED_MODE_NONE,
                      info->fl_dev_info);
    io_set_flash_ctrl(sd, SW_CTRL_FLASH_OFF, info->fl_dev_info);
  }
  #endif
  return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
  int ret=0;
  return ret;
}


/*
 * Store information about the video data format.
 */

static struct sensor_format_struct {
	__u8 *desc;
	//__u32 pixelformat;
	enum v4l2_mbus_pixelcode mbus_code;//linux-3.0
	struct regval_list *regs;
	int	regs_size;
	int bpp;   /* Bytes per pixel */
} sensor_formats[] = {
	{
		.desc		= "YUYV 4:2:2",
		.mbus_code	= V4L2_MBUS_FMT_YUYV8_2X8,//linux-3.0
		.regs 		= sensor_fmt_yuv422_yuyv,
		.regs_size = ARRAY_SIZE(sensor_fmt_yuv422_yuyv),
		.bpp		= 2,
	},
	{
		.desc		= "YVYU 4:2:2",
		.mbus_code	= V4L2_MBUS_FMT_YVYU8_2X8,//linux-3.0
		.regs 		= sensor_fmt_yuv422_yvyu,
		.regs_size = ARRAY_SIZE(sensor_fmt_yuv422_yvyu),
		.bpp		= 2,
	},
	{
		.desc		= "UYVY 4:2:2",
		.mbus_code	= V4L2_MBUS_FMT_UYVY8_2X8,//linux-3.0
		.regs 		= sensor_fmt_yuv422_uyvy,
		.regs_size = ARRAY_SIZE(sensor_fmt_yuv422_uyvy),
		.bpp		= 2,
	},
	{
		.desc		= "VYUY 4:2:2",
		.mbus_code	= V4L2_MBUS_FMT_VYUY8_2X8,//linux-3.0
		.regs 		= sensor_fmt_yuv422_vyuy,
		.regs_size = ARRAY_SIZE(sensor_fmt_yuv422_vyuy),
		.bpp		= 2,
	},
};


#define N_FMTS ARRAY_SIZE(sensor_formats)



/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */


static struct sensor_win_size sensor_win_sizes[] = {
	/* VGA */
	{
	  	.width      = VGA_WIDTH,
	  	.height     = VGA_HEIGHT,
	  	.hoffset    = 0,
	  	.voffset    = 0,
	  	.regs       = sensor_vga_regs,
	  	.regs_size  = ARRAY_SIZE(sensor_vga_regs),
	  	.set_size   = NULL,
	},
	/* QVGA */
	{
		.width		= QVGA_WIDTH,
		.height		= QVGA_HEIGHT,
		.hoffset	= 0,
		.voffset	= 0,
		.regs		= sensor_qvga_regs,
		.regs_size  = ARRAY_SIZE(sensor_qvga_regs),
		.set_size   = NULL,
	},
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_enum_fmt(struct v4l2_subdev *sd, unsigned index,
                 enum v4l2_mbus_pixelcode *code)
{
  if (index >= N_FMTS)
    return -EINVAL;

  *code = sensor_formats[index].mbus_code;
  return 0;
}

static int sensor_enum_size(struct v4l2_subdev *sd,
                            struct v4l2_frmsizeenum *fsize)
{
  if(fsize->index > N_WIN_SIZES-1)
  	return -EINVAL;

  fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
  fsize->discrete.width = sensor_win_sizes[fsize->index].width;
  fsize->discrete.height = sensor_win_sizes[fsize->index].height;

  return 0;
}


static int sensor_try_fmt_internal(struct v4l2_subdev *sd,
    struct v4l2_mbus_framefmt *fmt,
    struct sensor_format_struct **ret_fmt,
    struct sensor_win_size **ret_wsize)
{
  int index;
  struct sensor_win_size *wsize;

  for (index = 0; index < N_FMTS; index++)
    if (sensor_formats[index].mbus_code == fmt->code)
      break;
  //printk("format: %s\n",sensor_formats[index].desc);
  if (index >= N_FMTS)
    return -EINVAL;

  if (ret_fmt != NULL)
    *ret_fmt = sensor_formats + index;

  /*
   * Fields: the sensor devices claim to be progressive.
   */

  fmt->field = V4L2_FIELD_NONE;

  /*
   * Round requested image size down to the nearest
   * we support, but not below the smallest.
   */
  for (wsize = sensor_win_sizes; wsize < sensor_win_sizes + N_WIN_SIZES;
       wsize++)
    if (fmt->width >= wsize->width && fmt->height >= wsize->height)
      break;

  if (wsize >= sensor_win_sizes + N_WIN_SIZES)
    wsize--;   /* Take the smallest one */
  if (ret_wsize != NULL)
    *ret_wsize = wsize;
  /*
   * Note the size we'll actually handle.
   */
  fmt->width = wsize->width;
  fmt->height = wsize->height;
  //pix->bytesperline = pix->width*sensor_formats[index].bpp;
  //pix->sizeimage = pix->height*pix->bytesperline;

  return 0;
}

static int sensor_try_fmt(struct v4l2_subdev *sd,
             struct v4l2_mbus_framefmt *fmt)
{
  return sensor_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
           struct v4l2_mbus_config *cfg)
{
  cfg->type = V4L2_MBUS_PARALLEL;
  cfg->flags = V4L2_MBUS_MASTER | VREF_POL | HREF_POL | CLK_POL ;

  return 0;
}


/*
 * Set a format.
 */
static int sensor_s_fmt(struct v4l2_subdev *sd,
             struct v4l2_mbus_framefmt *fmt)
{
  int ret;
  struct sensor_format_struct *sensor_fmt;
  struct sensor_win_size *wsize;
  struct sensor_info *info = to_state(sd);

//  unsigned char rdval;
  vfe_dev_dbg("sensor_s_fmt\n");

//  sensor_write_array(sd, sensor_oe_disable_regs ,ARRAY_SIZE(sensor_oe_disable_regs));

  ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
  if (ret)
    return ret;

  if(info->capture_mode == V4L2_MODE_VIDEO)
  {
    //video
    #ifdef _FLASH_FUNC_
    if(info->flash_mode!=V4L2_FLASH_LED_MODE_NONE)
    {
      //printk("shut flash when preview\n");
      io_set_flash_ctrl(sd, SW_CTRL_FLASH_OFF, info->fl_dev_info);
    }
    #endif
  }
  else if(info->capture_mode == V4L2_MODE_IMAGE)
  {
#if 0
    //image
    ret = sensor_s_autoexp(sd,V4L2_EXPOSURE_MANUAL);
    if (ret < 0)
      vfe_dev_err("sensor_s_autoexp off err when capturing image!\n");

    ret = sensor_s_autogain(sd,0);
    if (ret < 0)
      vfe_dev_err("sensor_s_autogain off err when capturing image!\n");
    if (wsize->width > SVGA_WIDTH) {
      #ifdef _FLASH_FUNC_
      check_to_flash(sd);
      #endif
      sensor_get_lum(sd);
      sensor_get_preview_exposure(sd);
      sensor_get_fps(sd);
//      ret = sensor_set_capture_exposure(sd);
//      if (ret < 0)
//        vfe_dev_err("sensor_set_capture_exposure err !\n");
    }

    #ifdef _FLASH_FUNC_
    if(info->flash_mode!=V4L2_FLASH_LED_MODE_NONE)
    {
      if(to_flash==1)
      {
        vfe_dev_cap_dbg("open flash when capture\n");
        io_set_flash_ctrl(sd, SW_CTRL_FLASH_ON, info->fl_dev_info);
        sensor_get_lum(sd);
        sensor_get_preview_exposure(sd);
        sensor_get_fps(sd);
        msleep(50);
      }
    }
    #endif
    ret = sensor_s_autowb(sd,0); //lock wb
    if (ret < 0)
      vfe_dev_err("sensor_s_autowb off err when capturing image!\n");
#endif
  }

  sensor_write_array(sd, sensor_fmt->regs, sensor_fmt->regs_size);

  //printk("wsize->regs_size=%d\n", wsize->regs_size);
  if (wsize->regs)
    LOG_ERR_RET(sensor_write_array(sd, wsize->regs, wsize->regs_size))

  if (wsize->set_size)
    LOG_ERR_RET(wsize->set_size(sd))
#if 0

  sensor_s_hflip(sd,info->hflip);
  sensor_s_vflip(sd,info->vflip);
  if(info->capture_mode == V4L2_MODE_VIDEO ||
  	info->capture_mode == V4L2_MODE_PREVIEW)
  {
    ret = sensor_s_autoexp(sd,V4L2_EXPOSURE_AUTO);
    if (ret < 0)
      vfe_dev_err("sensor_s_autoexp on err when capturing video!\n");

    ret = sensor_s_autogain(sd,1);
    if (ret < 0)
      vfe_dev_err("sensor_s_autogain on err when capturing video!\n");

    if (info->wb == V4L2_WHITE_BALANCE_AUTO) {
      ret = sensor_s_autowb(sd,1); //unlock wb
      if (ret < 0)
        vfe_dev_err("sensor_s_autowb on err when capturing image!\n");
    }

		if(info->low_speed == 1) {
			if(info->preview_first_flag == 1) {
				info->preview_first_flag = 0;
				msleep(600);
			} else {
				msleep(200);
			}
		}

	}
#endif
  info->fmt = sensor_fmt;
  info->width = wsize->width;
  info->height = wsize->height;

  vfe_dev_print("s_fmt set width = %d, height = %d\n",wsize->width,wsize->height);

//	sensor_write_array(sd, sensor_oe_enable_regs, ARRAY_SIZE(sensor_oe_enable_regs));

	return 0;
}

/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->capturemode = info->capture_mode;

	cp->timeperframe.numerator = info->tpf.numerator;
	cp->timeperframe.denominator = info->tpf.denominator;

	return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
  struct v4l2_captureparm *cp = &parms->parm.capture;
  struct v4l2_fract *tpf = &cp->timeperframe;
  struct sensor_info *info = to_state(sd);
  unsigned char div;

  vfe_dev_dbg("sensor_s_parm\n");

  if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE){
  	vfe_dev_dbg("parms->type!=V4L2_BUF_TYPE_VIDEO_CAPTURE\n");
    return -EINVAL;
  }

  if (info->tpf.numerator == 0){
  	vfe_dev_dbg("info->tpf.numerator == 0\n");
    return -EINVAL;
  }

  info->capture_mode = cp->capturemode;

  if (info->capture_mode == V4L2_MODE_IMAGE) {
    vfe_dev_dbg("capture mode is not video mode,can not set frame rate!\n");
    return 0;
  }

  if (tpf->numerator == 0 || tpf->denominator == 0) {
    tpf->numerator = 1;
    tpf->denominator = SENSOR_FRAME_RATE;/* Reset to full rate */
    vfe_dev_err("sensor frame rate reset to full rate!\n");
  }

  div = SENSOR_FRAME_RATE/(tpf->denominator/tpf->numerator);
  if(div > 15 || div == 0)
  {
  	vfe_dev_print("SENSOR_FRAME_RATE=%d\n",SENSOR_FRAME_RATE);
  	vfe_dev_print("tpf->denominator=%d\n",tpf->denominator);
  	vfe_dev_print("tpf->numerator=%d\n",tpf->numerator);
    return -EINVAL;
  }

  vfe_dev_dbg("set frame rate %d\n",tpf->denominator/tpf->numerator);

  info->tpf.denominator = SENSOR_FRAME_RATE;
  info->tpf.numerator = div;

	if(info->tpf.denominator/info->tpf.numerator < 30)
		info->low_speed = 1;

  return 0;
}


/*
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function ,retrun -EINVAL
 */

/* *********************************************begin of ******************************************** */
static int sensor_queryctrl(struct v4l2_subdev *sd,
    struct v4l2_queryctrl *qc)
{
  /* Fill in min, max, step and default value for these controls. */
  /* see include/linux/videodev2.h for details */
//  vfe_dev_dbg("queryctrl qc->id=0x%8x\n", qc->id);
  switch (qc->id) {
//  case V4L2_CID_BRIGHTNESS:
//    return v4l2_ctrl_query_fill(qc, -4, 4, 1, 1);
//  case V4L2_CID_CONTRAST:
//    return v4l2_ctrl_query_fill(qc, -4, 4, 1, 1);
//  case V4L2_CID_SATURATION:
//    return v4l2_ctrl_query_fill(qc, -4, 4, 1, 1);
//  case V4L2_CID_HUE:
//    return v4l2_ctrl_query_fill(qc, -180, 180, 5, 0);
  case V4L2_CID_VFLIP:
  case V4L2_CID_HFLIP:
    return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
//  case V4L2_CID_GAIN:
//    return v4l2_ctrl_query_fill(qc, 0, 255, 1, 128);
//  case V4L2_CID_AUTOGAIN:
//    return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
  case V4L2_CID_EXPOSURE:
  case V4L2_CID_AUTO_EXPOSURE_BIAS:
    return v4l2_ctrl_query_fill(qc, -4, 4, 1, 0);
  case V4L2_CID_EXPOSURE_AUTO:
    return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
  case V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE:
    return v4l2_ctrl_query_fill(qc, 0, 9, 1, 1);
  case V4L2_CID_AUTO_WHITE_BALANCE:
    return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
  case V4L2_CID_COLORFX:
    return v4l2_ctrl_query_fill(qc, 0, 15, 1, 0);
  case V4L2_CID_FLASH_LED_MODE:
    return v4l2_ctrl_query_fill(qc, 0, 4, 1, 0);

  case V4L2_CID_AUTO_EXPOSURE_WIN_NUM:
    return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
  }
  return -EINVAL;
}

static int sensor_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
  //vfe_dev_dbg("sensor_g_ctrl ctrl->id=0x%8x\n", ctrl->id);
  switch (ctrl->id) {
  case V4L2_CID_BRIGHTNESS:
    return sensor_g_brightness(sd, &ctrl->value);
  case V4L2_CID_CONTRAST:
    return sensor_g_contrast(sd, &ctrl->value);
  case V4L2_CID_SATURATION:
    return sensor_g_saturation(sd, &ctrl->value);
  case V4L2_CID_HUE:
    return sensor_g_hue(sd, &ctrl->value);
  case V4L2_CID_VFLIP:
    return sensor_g_vflip(sd, &ctrl->value);
  case V4L2_CID_HFLIP:
    return sensor_g_hflip(sd, &ctrl->value);
  case V4L2_CID_GAIN:
    return sensor_g_gain(sd, &ctrl->value);
  case V4L2_CID_AUTOGAIN:
    return sensor_g_autogain(sd, &ctrl->value);
  case V4L2_CID_EXPOSURE:
  case V4L2_CID_AUTO_EXPOSURE_BIAS:
    return sensor_g_exp_bias(sd, &ctrl->value);
  case V4L2_CID_EXPOSURE_AUTO:
    return sensor_g_autoexp(sd, &ctrl->value);
  case V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE:
    return sensor_g_wb(sd, &ctrl->value);
  case V4L2_CID_AUTO_WHITE_BALANCE:
    return sensor_g_autowb(sd, &ctrl->value);
  case V4L2_CID_COLORFX:
    return sensor_g_colorfx(sd, &ctrl->value);
  case V4L2_CID_FLASH_LED_MODE:
    return sensor_g_flash_mode(sd, &ctrl->value);
  case V4L2_CID_AUTO_EXPOSURE_WIN_NUM:
  	ctrl->value=1;
  	return 0;
  }
  return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
  struct v4l2_queryctrl qc;
  int ret;

//  vfe_dev_dbg("sensor_s_ctrl ctrl->id=0x%8x\n", ctrl->id);
  qc.id = ctrl->id;
  ret = sensor_queryctrl(sd, &qc);
  if (ret < 0) {
    return ret;
  }

	if (qc.type == V4L2_CTRL_TYPE_MENU ||
		qc.type == V4L2_CTRL_TYPE_INTEGER ||
		qc.type == V4L2_CTRL_TYPE_BOOLEAN)
	{
	  if (ctrl->value < qc.minimum || ctrl->value > qc.maximum) {
	    return -ERANGE;
	  }
	}

  switch (ctrl->id) {
    case V4L2_CID_BRIGHTNESS:
      return sensor_s_brightness(sd, ctrl->value);
    case V4L2_CID_CONTRAST:
      return sensor_s_contrast(sd, ctrl->value);
    case V4L2_CID_SATURATION:
      return sensor_s_saturation(sd, ctrl->value);
    case V4L2_CID_HUE:
      return sensor_s_hue(sd, ctrl->value);
    case V4L2_CID_VFLIP:
      return sensor_s_vflip(sd, ctrl->value);
    case V4L2_CID_HFLIP:
      return sensor_s_hflip(sd, ctrl->value);
    case V4L2_CID_GAIN:
      return sensor_s_gain(sd, ctrl->value);
    case V4L2_CID_AUTOGAIN:
      return sensor_s_autogain(sd, ctrl->value);
    case V4L2_CID_EXPOSURE:
    case V4L2_CID_AUTO_EXPOSURE_BIAS:
      return sensor_s_exp_bias(sd, ctrl->value);
    case V4L2_CID_EXPOSURE_AUTO:
      return sensor_s_autoexp(sd,
          (enum v4l2_exposure_auto_type) ctrl->value);
    case V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE:
  		return sensor_s_wb(sd,
          (enum v4l2_auto_n_preset_white_balance) ctrl->value);
    case V4L2_CID_AUTO_WHITE_BALANCE:
      return sensor_s_autowb(sd, ctrl->value);
    case V4L2_CID_COLORFX:
      return sensor_s_colorfx(sd,
          (enum v4l2_colorfx) ctrl->value);
    case V4L2_CID_FLASH_LED_MODE:
      return sensor_s_flash_mode(sd,
          (enum v4l2_flash_led_mode) ctrl->value);
   // case V4L2_CID_POWER_LINE_FREQUENCY:
//      return sensor_s_band_filter(sd,
   //       (enum v4l2_power_line_frequency) ctrl->value);
    /*
    case V4L2_CID_3A_LOCK:
    	return sensor_s_3a_lock(sd, ctrl->value);
//    case V4L2_CID_AUTO_FOCUS_RANGE:
//  	  return 0;
	  case V4L2_CID_AUTO_FOCUS_INIT:
	  	return sensor_s_init_af(sd);
	  case V4L2_CID_AUTO_FOCUS_RELEASE:
	  	return sensor_s_release_af(sd);
	  case V4L2_CID_AUTO_FOCUS_START:
	  	return sensor_s_single_af(sd);
	  case V4L2_CID_AUTO_FOCUS_STOP:
	  	return sensor_s_pause_af(sd);
	//  case V4L2_CID_AUTO_FOCUS_STATUS:
	  case V4L2_CID_FOCUS_AUTO:
	  	return sensor_s_continueous_af(sd, ctrl->value);
	  case V4L2_CID_AUTO_FOCUS_WIN_NUM:
	  	vfe_dev_dbg("s_ctrl win value=%d\n",ctrl->value);
	  	return sensor_s_af_zone(sd, (struct v4l2_win_coordinate *)(ctrl->user_pt));*/
	  case V4L2_CID_AUTO_EXPOSURE_WIN_NUM:
	  	return 0;
  }
  return -EINVAL;
}


static int sensor_g_chip_ident(struct v4l2_subdev *sd,
    struct v4l2_dbg_chip_ident *chip)
{
  struct i2c_client *client = v4l2_get_subdevdata(sd);

  return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_SENSOR, 0);
}


/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops sensor_core_ops = {
  .g_chip_ident = sensor_g_chip_ident,
  .g_ctrl = sensor_g_ctrl,
  .s_ctrl = sensor_s_ctrl,
  .queryctrl = sensor_queryctrl,
  .reset = sensor_reset,
  .init = sensor_init,
  .s_power = sensor_power,
  .ioctl = sensor_ioctl,
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
  .enum_mbus_fmt = sensor_enum_fmt,
  .enum_framesizes = sensor_enum_size,
  .try_mbus_fmt = sensor_try_fmt,
  .s_mbus_fmt = sensor_s_fmt,
  .s_parm = sensor_s_parm,
  .g_parm = sensor_g_parm,
  .g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_ops sensor_ops = {
  .core = &sensor_core_ops,
  .video = &sensor_video_ops,
};

/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv = {
	.name = SENSOR_NAME,
};

static int sensor_probe(struct i2c_client *client,
      const struct i2c_device_id *id)
{
  struct v4l2_subdev *sd;
  struct sensor_info *info;
//  int ret;

  info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
  if (info == NULL)
    return -ENOMEM;
  sd = &info->sd;
  glb_sd = sd;
  cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);

  info->fmt = &sensor_formats[0];
  info->af_first_flag = 1;
  info->init_first_flag = 1;
  info->auto_focus = 0;

  return 0;
}

static int sensor_remove(struct i2c_client *client)
{
  struct v4l2_subdev *sd;

  sd = cci_dev_remove_helper(client, &cci_drv);
  kfree(to_state(sd));
  return 0;
}

static const struct i2c_device_id sensor_id[] = {
  { SENSOR_NAME, 0 },
  { }
};
MODULE_DEVICE_TABLE(i2c, sensor_id);

static struct i2c_driver sensor_driver = {
  .driver = {
    .owner = THIS_MODULE,
  .name = SENSOR_NAME,
  },
  .probe = sensor_probe,
  .remove = sensor_remove,
  .id_table = sensor_id,
};

static __init int init_sensor(void)
{
	return cci_dev_init_helper(&sensor_driver);
}

static __exit void exit_sensor(void)
{
	cci_dev_exit_helper(&sensor_driver);
}

module_init(init_sensor);
module_exit(exit_sensor);

