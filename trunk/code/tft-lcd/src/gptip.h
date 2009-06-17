/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : gptip.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 05/31/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _GPTIP_H_
#define	_GPTIP_H_


#define	GPTIP_THL			32
#define	XDATA_MAX_LEN		1024
/***------------------------------------*/
#define	CMD_LEADER1			0xFF
#define	CMD_LEADER2			0x5A

enum CMD_MODE
{
	 	GET_REQUEST		= 0,
		SET_REQUEST		= 1,
		SET_ACK			= 2,
		GET_ACK			= 3,
		EVENT			= 4,
};

typedef struct	_GptHead
{
	vu8		head_a;
	vu8		head_b;
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
	vu8		xdata[XDATA_MAX_LEN];
}GPTIP, *pGPTIP;

#endif
/******************************END OF FILE**************************************/
