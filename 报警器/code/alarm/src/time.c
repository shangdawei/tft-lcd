/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: time.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/27  || Create this file
**  ******************************************************************************************
*/
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
//TIMER0 initialize - prescale:1024 /*定时器预分频，预分频由TCCRn的CS02,CS01,CS00确定，详情查看数据手册*/

// WGM: Normal/*定时器,也由TCCRn确定*/

// desired value: 20mSec/*定时器期望设定时间*/

// actual value: 19.861mSec (0.7%)/*定时器实际定时时间，误差比例*/

void timer0_init(void)
{

/*定时器停止，TCCR0寄存器完全控制timer0的运行情况，详细可参考数据手册。*/
    TCCR0A = 0x02; //stop
    TCCR0B = 0x00;//stop
      
    TCNT0 = 0x71; //set count /*定时器寄存器开始值*/
    OCR0A  = 0x8F;  //set compare /*定时器比较值*/
    
    TIMSK0 = 0x02;//输出比较匹配A 中断使能
    TCCR0B = 0x01; //start timer /*定时器开始*/
}

#pragma vector = TIMER0_COMPA_vect
__interrupt void timer0_comp_isr(void)
{
    //compare occured TCNT0=OCR0
    TCNT0 = 0x71; //set count /*定时器寄存器开始值*/
    OCR0A  = 0x8F;  //set compare /*定时器比较值*/
  
  count100ms++;
   count500ms++;
   count1000ms++;
   count2000ms++;
   count5000ms++;

   if(count100ms>10){
     count100ms=0;
     flag_100ms=1;
   }
   if(count500ms>50){
   count500ms=0;
   flag_500ms=1;
   }
   if(count1000ms>100){
   count1000ms=0;
   flag_1000ms=1;
   }
   if(count2000ms>200){
   count2000ms=0;
   flag_2000ms=1;
   }
   if(count5000ms>500){
   count5000ms=0;
   flag_5000ms=1;
   
}

}






/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
