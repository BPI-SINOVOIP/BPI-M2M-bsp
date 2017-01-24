/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "OSAL_Power.h"


#ifndef __OSAL_POWER_MASK__

int OSAL_Power_Enable(char *name)
{
	struct regulator *regu= NULL;
	int ret = 0;
	regu= regulator_get(NULL, name);
	if (IS_ERR(regu)) {
		__wrn("%s: some error happen, fail to get regulator %s\n", __func__, name);
		goto exit;
	}

	//enalbe regulator
	ret = regulator_enable(regu);
	if (0 != ret) {
		__wrn("%s: some error happen, fail to enable regulator %s!\n", __func__, name);
		goto exit1;
	} else {
		__inf("suceess to enable regulator %s!\n", name);
	}

exit1:
	//put regulater, when module exit
	regulator_put(regu);
exit:
	return ret;
}

int OSAL_Power_Disable(char *name)
{
	struct regulator *regu= NULL;
	int ret = 0;
	regu= regulator_get(NULL, name);
	if (IS_ERR(regu)) {
		__wrn("%s: some error happen, fail to get regulator %s\n", __func__, name);
		goto exit;
	}

	//disalbe regulator
	ret = regulator_disable(regu);
	if (0 != ret) {
		__wrn("%s: some error happen, fail to disable regulator %s!\n", __func__, name);
		goto exit1;
	} else {
		__inf("suceess to disable regulator %s!\n", name);
	}

exit1:
	//put regulater, when module exit
	regulator_put(regu);
exit:
	return ret;
}

#else

int OSAL_Power_Enable(char *name)
{
	int ret = 0;

	return ret;
}

int OSAL_Power_Disable(char *name)
{
	int ret = 0;

	return ret;
}

#endif
