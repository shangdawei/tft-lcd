/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : gptip.h
* Author             : MCD Application Team
* Version            : V0.1
* Date               : 05/31/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _GPTIP_H_
#define	_GPTIP_H_

#define	MAX_LEN		1024

typedef struct
{
	vu16	head;
	vu16	cmd;
	vu8		ver;
	vu16	devid;
	vu8		thl;
	vu16	extlen;
	vu8		type;
	vu16	crc16;
	vu8		res[3];
	vu8		data[16];
}GptHead, *pGptHead;

typedef struct
{
	GptHead head;
	vu8		xdata[MAX_LEN];
}Gptip, *pGptip;

#endif
/******************************END OF FILE**************************************/
