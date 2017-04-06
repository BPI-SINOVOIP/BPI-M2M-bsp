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
#include "pm_i.h"

#if defined(CONFIG_ARCH_SUN9IW1P1) || defined(CONFIG_ARCH_SUN8IW6P1) \
	|| defined(CONFIG_ARCH_SUN8IW9P1)
static __mem_tmstmp_reg_t  *TmstmpReg;

/*
*********************************************************************************************************
*                                     TIMESTAMP save
*
* Description: save timestamp for mem.
*
* Arguments  : none
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 mem_tmstmp_save(__mem_tmstmp_reg_t *ptmstmp_state)
{

	/* set timestamp register base */
	TmstmpReg = (__mem_tmstmp_reg_t *)IO_ADDRESS(SUNXI_TIMESTAMP_CTRL_PBASE);
	
	/* backup timestamp registers */
	ptmstmp_state->Ctl		    = TmstmpReg->Ctl;
	ptmstmp_state->Cluster0CtrlReg1     = TmstmpReg->Cluster0CtrlReg1;
	
	return 0;
}


/*
*********************************************************************************************************
*                                     TIMESTAMP restore
*
* Description: restore timestamp for mem.
*
* Arguments  : none
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 mem_tmstmp_restore(__mem_tmstmp_reg_t *ptmstmp_state)
{
	/* restore timestamp0 parameters */
	TmstmpReg->Ctl			= ptmstmp_state->Ctl;
	TmstmpReg->Cluster0CtrlReg1	= ptmstmp_state->Cluster0CtrlReg1;
	
	return 0;
}
#endif

