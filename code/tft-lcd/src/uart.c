/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : uart.c
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/06/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#define		_UART_C_
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "uart.h"
#include "gptip.h"
#include <string.h>
/* Private variable-------------------------------------------------------*/
static stcBuf	UartRcvBuf;
/*********************************************************************************************************  
**	$Function   :   SetupUSART
**	----------------------------------------------------------------------------------------------
**  Description :  	SetupUSART
**	----------------------------------------------------------------------------------------------
**  Argument    :   
**	----------------------------------------------------------------------------------------------
**  Return      :   
**              :   
**	----------------------------------------------------------------------------------------------
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/06/06  || Create this function
**	----------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetupUSART(void)
{
	memset(&UartRcvBuf, 0, sizeof(stcBuf));
}


/*********************************************************************************************************  
**	$Function   :   SendChar
**	----------------------------------------------------------------------------------------------
**  Description :  	SendChar to uart
**	----------------------------------------------------------------------------------------------
**  Argument    :   
**	----------------------------------------------------------------------------------------------
**  Return      :   
**              :   
**	----------------------------------------------------------------------------------------------
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/06/06  || Create this function
**	----------------------------------------------------------------------------------------------
********************************************************************************************************/
u8 SendChar (USART_TypeDef *USARTx, u8 ch)  
{              
/* Write character to Serial Port     */

  USART_SendData(USARTx, (unsigned char) ch);
  while (!(USARTx->SR & USART_FLAG_TXE));
  return (ch);
}

void SendString(USART_TypeDef *USARTx, char* s)
{
   while(*s != '\0')
   {
	   SendChar(USARTx, *s);
	   s++;
   }
}

void SendLine(USART_TypeDef *USARTx, char* s, u16 len)
{
   u16 i;
   for(i = 0; i < len; i++)
   {
	   SendChar(USARTx, s[i]);
   }
}
/*********************************************************************************************************  
**	$Function   :   URecvData
**	----------------------------------------------------------------------------------------------
**  Description :  	URecvData from uart	,for uart1 handle
**	----------------------------------------------------------------------------------------------
**  Argument    :   
**	----------------------------------------------------------------------------------------------
**  Return      :   
**              :   
**	----------------------------------------------------------------------------------------------
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/06/06  || Create this function
**	----------------------------------------------------------------------------------------------
********************************************************************************************************/
void URecvData(void)
{
 	P_stcBuf	pBuf = &UartRcvBuf;
	u8 data;
	data = USART_ReceiveData(USART1);
	if(0 == pBuf->FullFlag)
	{ 
		pBuf->Buf[pBuf->WritePoint] = data;
		pBuf->WritePoint++;
		pBuf->WritePoint &= BUF_MASK;
		if(pBuf->WritePoint == pBuf->ReadPoint)
		{
			pBuf->FullFlag = 1;
		}
	}
}
/*********************************************************************************************************  
**	$Function   :   ReadFromComBuf
**	----------------------------------------------------------------------------------------------
**  Description :  	read data from uart
**	----------------------------------------------------------------------------------------------
**  Argument    :   
**	----------------------------------------------------------------------------------------------
**  Return      :   1-ok
**              :   0-not prepare
**	----------------------------------------------------------------------------------------------
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/06/06  || Create this function
**	----------------------------------------------------------------------------------------------
********************************************************************************************************/
u8 ReadFromComBuf(pGPTIP pGptip)
{
	P_stcBuf pRcvBuf = &UartRcvBuf;
	static u8 sRxdSta = U0_RX_STA_LEADER1;			// 接收状态
	u8* p = (u8*)pGptip;
	static u16 i, thl, extlen;
	while((0 != pRcvBuf->FullFlag) || (pRcvBuf->WritePoint != pRcvBuf->ReadPoint))
	{
		switch(sRxdSta)
		{
			case U0_RX_STA_LEADER1:	// 第一引导码正确
				i = 0;
				if(pRcvBuf->Buf[pRcvBuf->ReadPoint] == CMD_LEADER1)
				{
					sRxdSta = U0_RX_STA_LEADER2;
					p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				}
				break;
			case U0_RX_STA_LEADER2:	// 第二引导码正确
				if (pRcvBuf->Buf[pRcvBuf->ReadPoint] == CMD_LEADER2)
				{
					sRxdSta = U0_RX_STA_THL;
					p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				}
				else
				{
					sRxdSta = U0_RX_STA_LEADER1;
				}
				break;
			case U0_RX_STA_THL:	// 包头长度
				thl = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				sRxdSta = U0_RX_STA_EXTLEN_L;
				break;
			case U0_RX_STA_EXTLEN_L: //扩展长度低字节
				extlen = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				sRxdSta = U0_RX_STA_EXTLEN_H;
				break;
			case U0_RX_STA_EXTLEN_H: //扩展长度高字节
				extlen = ((u16)pRcvBuf->Buf[pRcvBuf->ReadPoint] << 8) | extlen;
				p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				if((thl + extlen) > sizeof(GPTIP) || thl != sizeof(GptHead))
				{
				   sRxdSta = U0_RX_STA_LEADER1;
				}
				else
				{
					sRxdSta = U0_RX_STA_DATA;
				}
				break;
			case U0_RX_STA_DATA:	//接收数据
				p[i++] = pRcvBuf->Buf[pRcvBuf->ReadPoint];
				if(i ==  (thl + extlen))
				{
				 	sRxdSta = U0_RX_STA_LEADER1;
					return 1;
				}
				break;
			default:
				break;

		}
		pRcvBuf->FullFlag = 0;
		pRcvBuf->ReadPoint++;
		pRcvBuf->ReadPoint &= BUF_MASK;
	}
	return 0;
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

