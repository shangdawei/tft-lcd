//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file	time.c
/// @brief	��ʱ������
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2009-09-27                 
///
///
///     �޶�˵��������汾
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
///     ��ʱ��0��ʼ����10ms�ж�һ��
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
void timer0_init(void)
{
    TCCR0A = 0x00;//stop
    TCCR0B = 0x00;//stop  

	/*1/1024��Ƶ��10ms��Ҫ78��ʱ������
	  �����ֵΪ0xff - 78 = 0xB1*/
	TCNT0 = 0xB1; //set count /*��ʱ���Ĵ�����ʼֵ*/
    
    TIMSK0 = 0x01;//����ж�ʹ�� �ж�ʹ��

	TCCR0B = 0x05;//������ʱ����1/8��Ƶ
}

//////////////////////////////////////////////////////////////////////////
///
///     ��ʱ��0�жϷ����ӳ���
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
#pragma vector = TIMER0_OVF_vect
__interrupt void timer0_ovf_isr(void)
{

    TCNT0 = 0xB1; //set count /*��ʱ���Ĵ�����ʼֵ*/
  
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
