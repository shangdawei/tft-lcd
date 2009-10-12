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
//TIMER0 initialize - prescale:1024 /*��ʱ��Ԥ��Ƶ��Ԥ��Ƶ��TCCRn��CS02,CS01,CS00ȷ��������鿴�����ֲ�*/

// WGM: Normal/*��ʱ��,Ҳ��TCCRnȷ��*/

// desired value: 20mSec/*��ʱ�������趨ʱ��*/

// actual value: 19.861mSec (0.7%)/*��ʱ��ʵ�ʶ�ʱʱ�䣬������*/

void timer0_init(void)
{

/*��ʱ��ֹͣ��TCCR0�Ĵ�����ȫ����timer0�������������ϸ�ɲο������ֲᡣ*/
    TCCR0A = 0x02; //stop
    TCCR0B = 0x00;//stop
      
    TCNT0 = 0x71; //set count /*��ʱ���Ĵ�����ʼֵ*/
    OCR0A  = 0x8F;  //set compare /*��ʱ���Ƚ�ֵ*/
    
    TIMSK0 = 0x02;//����Ƚ�ƥ��A �ж�ʹ��
    TCCR0B = 0x01; //start timer /*��ʱ����ʼ*/
}

#pragma vector = TIMER0_COMPA_vect
__interrupt void timer0_comp_isr(void)
{
    //compare occured TCNT0=OCR0
    TCNT0 = 0x71; //set count /*��ʱ���Ĵ�����ʼֵ*/
    OCR0A  = 0x8F;  //set compare /*��ʱ���Ƚ�ֵ*/
  
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
