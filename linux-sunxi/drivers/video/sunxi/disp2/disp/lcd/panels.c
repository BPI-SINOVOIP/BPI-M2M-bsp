#include "panels.h"

struct sunxi_lcd_drv g_lcd_drv;

extern __lcd_panel_t default_panel;
extern __lcd_panel_t lt070me05000_panel;
extern __lcd_panel_t wtq05027d01_panel;
extern __lcd_panel_t hp784bl0292_panel;
extern __lcd_panel_t v920tvv01a_panel;
extern __lcd_panel_t t27p06_panel;
extern __lcd_panel_t st7789s_panel;
extern __lcd_panel_t tft720x1280_panel;
extern __lcd_panel_t ibt_panel;
extern __lcd_panel_t sl006p_panel;

__lcd_panel_t* panel_array[] = {
	&default_panel,
	&lt070me05000_panel,
	&wtq05027d01_panel,
	&hp784bl0292_panel,
	&v920tvv01a_panel,
	&t27p06_panel,
	&st7789s_panel,
	&tft720x1280_panel,
	&ibt_panel,
	/* add new panel below */
        &sl006p_panel,
	NULL,
};

static void lcd_set_panel_funs(void)
{
	int i;

	for(i=0; panel_array[i] != NULL; i++) {
		sunxi_lcd_set_panel_funs(panel_array[i]->name, &panel_array[i]->func);
	}

	return ;
}

int lcd_init(void)
{
	sunxi_disp_get_source_ops(&g_lcd_drv.src_ops);
	lcd_set_panel_funs();

	return 0;
}
