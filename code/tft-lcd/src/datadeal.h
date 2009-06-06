/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : datadeal.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/06/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _DATADEAL_H_
#define	_DATADEAL_H_

#ifndef	_DATADEAL_C_
#define	GLOBLE_DATADEAL		extern
#else
#define	GLOBLE_DATADEAL
#endif

GLOBLE_DATADEAL u8 CRC_Check(pGPTIP pGptip);
GLOBLE_DATADEAL void DealCmd(pGPTIP pGptip);
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
