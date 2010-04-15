//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file	time.c
/// @brief	定时器处理
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2009-09-27                 
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////
#define _TIME_C_
#include "def.h"


uint8 		count100ms=0;
uint8 		flag_100ms=0;
uint8       flag_500ms=0;
uint8       count500ms=0;
uint8		flag_1000ms=0;
uint8		count1000ms=0;
uint8		flag_2000ms=0;
uint8		count2000ms=0;
uint8		flag_5000ms=0;
uint16		count5000ms=0;

//////////////////////////////////////////////////////////////////////////
///
///     定时器0初始化，10ms中断一次
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
void timer0_init(void)
{
    TCCR0A = 0x00;//stop
    TCCR0B = 0x00;//stop  

	/*1/1024分频，10ms需要78个时钟周期
	  输入的值为0xff - 78 = 0xB1*/
	TCNT0 = 0xB1; //set count /*定时器寄存器开始值*/
    
    TIMSK0 = 0x01;//溢出中断使能 中断使能

	TCCR0B = 0x05;//启动定时器，1/8分频
}

//////////////////////////////////////////////////////////////////////////
///
///     定时器0中断服务子程序
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
#pragma vector = TIMER0_OVF_vect
__interrupt void timer0_ovf_isr(void)
{

    TCNT0 = 0xB1; //set count /*定时器寄存器开始值*/
  
	count100ms++;
	count500ms++;
	count1000ms++;
	count2000ms++;
	count5000ms++;

	if(count100ms>10)
	{
		count100ms=0;
		flag_100ms=1;
	}
	if(count500ms>50)
	{
		count500ms=0;
		flag_500ms=1;
	}
	if(count1000ms>100)
	{
	   count1000ms=0;
	   flag_1000ms=1;
	}
	if(count2000ms>200)
	{
	   count2000ms=0;
	   flag_2000ms=1;
	}
	if(count5000ms>500)
	{
	   count5000ms=0;
	   flag_5000ms=1;   
	}
}






/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
