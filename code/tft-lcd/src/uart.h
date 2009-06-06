/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : uart.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/06/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/

#ifndef		_UART_H_
#define		_UART_H_
#include "gptip.h"
#ifndef		_UART_C_
#define		GLOBLE_UART		extern
#else
#define		GLOBLE_UART
#endif

#define		UART_MAX_LEN	1024*2
#define		BUF_MASK		UART_MAX_LEN - 1
/** -----------------------------------*/
#define	U0_RX_STA_LEADER1	0x08
#define	U0_RX_STA_LEADER2	0x09
#define	U0_RX_STA_THL		0x10
#define	U0_RX_STA_EXTLEN_L	0x11
#define	U0_RX_STA_EXTLEN_H	0x12
#define	U0_RX_STA_DATA		0x13

typedef		struct	 _stcBuf
{
	u8	Buf[UART_MAX_LEN];	//UART接收缓冲区
	u16	WritePoint;			//UART的写指针
	u16	ReadPoint;			//UART的读指针
	u8	FullFlag;			//UART缓冲区满标志
} stcBuf,*P_stcBuf;

/* Globle variable ------------------------------------------------------------------*/
GLOBLE_UART	GPTIP	Gptip;
GLOBLE_UART	void SetupUSART(void);
GLOBLE_UART	u8 ReadFromComBuf(pGPTIP pGptip);

#endif
