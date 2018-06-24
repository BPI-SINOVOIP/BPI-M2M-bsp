#include "panels.h"
#include "S070WV20_MIPI_RGB.h"

#define panel_rst(v)    (sunxi_lcd_gpio_set_value(0, 0, v))
#define panel_pwr(v)    (sunxi_lcd_gpio_set_value(0, 1, v))

static void LCD_power_on(u32 sel);
static void LCD_power_off(u32 sel);
static void LCD_bl_open(u32 sel);
static void LCD_bl_close(u32 sel);

static void LCD_panel_init(u32 sel);
static void LCD_panel_exit(u32 sel);

extern void sunxi_lcd_tcon_enable(u32 screen_id);
extern void sunxi_lcd_tcon_disable(u32 screen_id);
extern void sunxi_lcd_power_enable(u32 screen_id, u32 pwr_id);
extern void sunxi_lcd_power_disable(u32 screen_id, u32 pwr_id);
extern void sunxi_lcd_backlight_enable(u32 screen_id);
extern void sunxi_lcd_backlight_disable(u32 screen_id);
extern s32 sunxi_lcd_pwm_enable(u32 pwm_channel);
extern s32 sunxi_lcd_pwm_disable(u32 pwm_channel);
extern s32 sunxi_lcd_pin_cfg(u32 screen_id, u32 bon);
extern void LCD_OPEN_FUNC(u32 screen_id, LCD_FUNC func, u32 delay);
extern void LCD_CLOSE_FUNC(u32 screen_id, LCD_FUNC func, u32 delay);

static void LCD_cfg_panel_info(panel_extend_para * info)
{
	u32 i = 0, j=0;
	u32 items;
	u8 lcd_gamma_tbl[][2] =
	{
		//{input value, corrected value}
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
		{LCD_CMAP_G0,LCD_CMAP_B1,LCD_CMAP_G2,LCD_CMAP_B3},
		{LCD_CMAP_B0,LCD_CMAP_R1,LCD_CMAP_B2,LCD_CMAP_R3},
		{LCD_CMAP_R0,LCD_CMAP_G1,LCD_CMAP_R2,LCD_CMAP_G3},
		},
		{
		{LCD_CMAP_B3,LCD_CMAP_G2,LCD_CMAP_B1,LCD_CMAP_G0},
		{LCD_CMAP_R3,LCD_CMAP_B2,LCD_CMAP_R1,LCD_CMAP_B0},
		{LCD_CMAP_G3,LCD_CMAP_R2,LCD_CMAP_G1,LCD_CMAP_R0},
		},
	};

	items = sizeof(lcd_gamma_tbl)/2;
	for (i=0; i<items-1; i++) {
		u32 num = lcd_gamma_tbl[i+1][0] - lcd_gamma_tbl[i][0];

		for (j=0; j<num; j++) {
			u32 value = 0;

			value = lcd_gamma_tbl[i][1] + ((lcd_gamma_tbl[i+1][1] - lcd_gamma_tbl[i][1]) * j)/num;
			info->lcd_gamma_tbl[lcd_gamma_tbl[i][0] + j] = (value<<16) + (value<<8) + value;
		}
	}
	info->lcd_gamma_tbl[255] = (lcd_gamma_tbl[items-1][1]<<16) + (lcd_gamma_tbl[items-1][1]<<8) + lcd_gamma_tbl[items-1][1];

	memcpy(info->lcd_cmap_tbl, lcd_cmap_tbl, sizeof(lcd_cmap_tbl));

}

static s32 LCD_open_flow(u32 sel)
{
	printf("[BPI]LCD_open_flow\n");
	
	LCD_OPEN_FUNC(sel, LCD_power_on, 200);   //open lcd power, and delay 50ms
	LCD_OPEN_FUNC(sel, LCD_panel_init, 150);   //open lcd power, than delay 200ms
    LCD_OPEN_FUNC(sel, sunxi_lcd_tcon_enable, 300);     //open lcd controller, and delay 100ms
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);     //open lcd backlight, and delay 0ms

	return 0;
}

static s32 LCD_close_flow(u32 sel)
{
	printf("[BPI]LCD_close_flow\n");
	LCD_CLOSE_FUNC(sel, LCD_bl_close, 0);       //close lcd backlight, and delay 0ms
	LCD_CLOSE_FUNC(sel, sunxi_lcd_tcon_disable, 0);         //close lcd controller, and delay 0ms
	LCD_CLOSE_FUNC(sel, LCD_panel_exit,	20);   //open lcd power, than delay 200ms
	LCD_CLOSE_FUNC(sel, LCD_power_off, 500);   //close lcd power, and delay 500ms

	return 0;
}

static void LCD_power_on(u32 sel)
{
	printf("[BPI]LCD_power_on\n");
	sunxi_lcd_power_enable(sel, 0);//config lcd_power pin to open lcd power
    sunxi_lcd_pin_cfg(sel, 1);
}

static void LCD_power_off(u32 sel)
{
	printf("[BPI]LCD_power_off\n");
	sunxi_lcd_power_disable(sel, 0);//config lcd_power pin to close lcd power
	sunxi_lcd_pin_cfg(sel, 0);
}

static void LCD_bl_open(u32 sel)
{
	printf("[BPI]LCD_bl_open\n");
	sunxi_lcd_pwm_enable(sel);
	sunxi_lcd_backlight_enable(sel);//config lcd_bl_en pin to open lcd backlight
}

static void LCD_bl_close(u32 sel)
{
	printf("[BPI]LCD_bl_close\n");
	sunxi_lcd_backlight_disable(sel);//config lcd_bl_en pin to close lcd backlight
	sunxi_lcd_pwm_disable(sel);
}

#define REGFLAG_END_OF_TABLE     0x102
#define REGFLAG_DELAY            0x101

struct lcd_setting_table {
    u16 cmd;
    u8 para;
};

static struct lcd_setting_table lcm_initialization_setting[] = {
		{0x7A, 0xC1},
		{0x20, 0x20},
		{0x21, 0xE0},
		{0x22, 0x13},
		{0x23, 0x28},
		{0x24, 0x30},
		{0x25, 0x28},
        {0x26, 0x00},
        {0x27, 0x0D},
        {0x28, 0x03},
        {0x29, 0x1D},
		{0x34, 0x80},
        {0x36, 0x28},
        {0xB5, 0xA0},
        {0x5C, 0xFF},
        {0x2A, 0x01},
		{0x56, 0x92},
        {0x6B, 0x71},
        {0x69, 0x2B},
        {0x10, 0x40},
        {0x11, 0x98},
		{0xB6, 0x20},
        {0x51, 0x20},
        {0x09, 0x10},
		{REGFLAG_DELAY, 20},
    	{REGFLAG_END_OF_TABLE, 00}
};

static void LCD_panel_init(u32 sel)
{
	u32 i;
	
	printf("[BPI]LCD_panel_init\n");

    panel_pwr(1);
    sunxi_lcd_delay_ms(50);
    panel_rst(1);
    sunxi_lcd_delay_ms(10);
    panel_rst(0);
    sunxi_lcd_delay_ms(30);
    panel_rst(1);
    sunxi_lcd_delay_ms(20);

	for (i = 0; ; i++) {
        if(lcm_initialization_setting[i].cmd == REGFLAG_END_OF_TABLE) {
            break;
        } else if (lcm_initialization_setting[i].cmd == REGFLAG_DELAY) {
            sunxi_lcd_delay_ms(lcm_initialization_setting[i].para);
        } else {
            sunxi_lcd_dsi_gen_write_1para(sel, (u8)lcm_initialization_setting[i].cmd, lcm_initialization_setting[i].para);
			sunxi_lcd_delay_ms(10);
        }
    }

    sunxi_lcd_dsi_clk_enable(sel);

	return;
}

static void LCD_panel_exit(u32 sel)
{
    panel_pwr(0);
    panel_rst(0);
    sunxi_lcd_dsi_clk_disable(sel);

	return ;
}

//sel: 0:lcd0; 1:lcd1
static s32 LCD_user_defined_func(u32 sel, u32 para1, u32 para2, u32 para3)
{
	return 0;
}

__lcd_panel_t S070WV20_MIPI_RGB_panel = {
	/* panel driver name, must mach the name of lcd_drv_name in sys_config.fex */
	.name = "S070WV20_MIPI_RGB",
	.func = {
		.cfg_panel_info = LCD_cfg_panel_info,
		.cfg_open_flow = LCD_open_flow,
		.cfg_close_flow = LCD_close_flow,
		.lcd_user_defined_func = LCD_user_defined_func,
	},
};
