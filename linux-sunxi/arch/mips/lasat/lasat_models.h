/*
 * arch/mips/lasat/lasat_models.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * Model description tables
 */
#include <linux/kernel.h>

struct product_info {
	const char     *pi_name;
	const char     *pi_type;
};

struct vendor_info {
	const char     *vi_name;
	const struct product_info *vi_product_info;
};

/*
 * Base models
 */
static const char * const txt_base_models[] = {
	"MQ 2", "MQ Pro", "SP 25", "SP 50", "SP 100", "SP 5000", "SP 7000",
	"SP 1000", "Unknown"
};
#define N_BASE_MODELS (ARRAY_SIZE(txt_base_models) - 1)

/*
 * Eicon Networks
 */
static const char txt_en_mq[] = "Masquerade";
static const char txt_en_sp[] = "Safepipe";

static const struct product_info product_info_eicon[] = {
	{ txt_en_mq, "II"   }, /*  0 */
	{ txt_en_mq, "Pro"  }, /*  1 */
	{ txt_en_sp, "25"   }, /*  2 */
	{ txt_en_sp, "50"   }, /*  3 */
	{ txt_en_sp, "100"  }, /*  4 */
	{ txt_en_sp, "5000" }, /*  5 */
	{ txt_en_sp, "7000" }, /*  6 */
	{ txt_en_sp, "30"   }, /*  7 */
	{ txt_en_sp, "5100" }, /*  8 */
	{ txt_en_sp, "7100" }, /*  9 */
	{ txt_en_sp, "1110" }, /* 10 */
	{ txt_en_sp, "3020" }, /* 11 */
	{ txt_en_sp, "3030" }, /* 12 */
	{ txt_en_sp, "5020" }, /* 13 */
	{ txt_en_sp, "5030" }, /* 14 */
	{ txt_en_sp, "1120" }, /* 15 */
	{ txt_en_sp, "1130" }, /* 16 */
	{ txt_en_sp, "6010" }, /* 17 */
	{ txt_en_sp, "6110" }, /* 18 */
	{ txt_en_sp, "6210" }, /* 19 */
	{ txt_en_sp, "1020" }, /* 20 */
	{ txt_en_sp, "1040" }, /* 21 */
	{ txt_en_sp, "1050" }, /* 22 */
	{ txt_en_sp, "1060" }, /* 23 */
};

#define N_PRIDS ARRAY_SIZE(product_info_eicon)

/*
 * The vendor table
 */
static struct vendor_info const vendor_info_table[] = {
	{ "Eicon Networks",	product_info_eicon   },
};

#define N_VENDORS ARRAY_SIZE(vendor_info_table)
