/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: phone.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#define _PHONE_C_

#include "def.h"
#include "phone.h"


bool g_bDetRingFst = 0;//ring come flag

static void clk_h(void)
{
	PORTC |= DIAL_CLK;
}

static void clk_l(void)
{
	PORTC &= ~DIAL_CLK;
}

static void put_value(uint8 value)
{
	if(value >= 1)
		PORTC |= DIAL_DATA;
	else
		PORTC &= ~DIAL_DATA;
}

static void enable_cs(void)
{
	DDRC |= DIAL_CS;	//output 
	PORTC &= ~DIAL_CS;	//put 0,enable CS
}
static void disable_cs(void)
{
	DDRC |= DIAL_CS;	//output 
	PORTC |= DIAL_CS;	//put 0,disable CS
}

void put_9200a(uint8 value)
{
	int8 i;
	
//	PORTC &= !DIAL_CS;	//put 0,enable CS
	
	for(i = 4; i >= 0 ; i--)
	{
		clk_h();
		put_value((value >> i) & 0x01);
		__delay_cycles(20);
		clk_l();
		__delay_cycles(20);
	}
	 __delay_cycles(20);
	__delay_cycles(20);
//	PORTC |= DIAL_CS;
	
}

void init_dial(void)
{
	DDRC |= DIAL_CS | DIAL_CLK | DIAL_DATA;	//output 
}

void dial_num(uint8 *p, uint8 num)
{
	uint8 i;
	init_dial();
	enable_cs();
	for(i = 0; i < num; i++)
		put_9200a(p[i]);
	put_9200a(0xFF);	//stop code
	disable_cs();
}

/*  $Function   :   call_the_phone
==  ==============================================================================================
==  Description :   拨打6组电话号码
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date				||     Contents
==						  :   xul      ||			    ||   2009/11/5    || Create this function
==  ===============================================================================================
*/
int call_the_phone(STWORK* pInfo)
{
	int8 i,j;
	init_dial();
	for (i = 0; i < MAX_TEL_NUM; i++)
	{
		j = 0;
		while (pInfo->phone_num[i][j] != '\0')
		{
			j++;
		}
		dial_num(pInfo->phone_num[i], j);
		//这里怎么检测对方电话是否接通？？？？
	}

	return 1;//需要修改
}
/*----------------------------------------------------------------------------
---------------------------phone receive mode---------------------------------
------------------------------------------------------------------------------*/

uint8 dtmf_read(void)
{
  char val=0;
  val = CSC9270_D2_D0_STA;
  val |= CSC9270_D3_STA;
  val &= 0x0f;
  /*
  if(val >= sizeof(csc9270_dtmf_tab))
    return 0xff;
  */
  return csc9270_dtmf_tab[val];
}

uint8 dtmf_detect(void)
{
  uint8 val = 0xff;
  
  /*set D3-D0 input mode*/
  CSC9270_D2_D0_DIR &= ~(CSC9270_D0 | CSC9270_D1 | CSC9270_D2);
  CSC9270_D3_DIR &= ~CSC9270_D3;
  
  CSC9270_DV_DIR &= ~CSC9270_DV_PIN;//set input mode
  CSC9270_OE_DIR |= CSC9270_OE;//set output mode
  CSC9270_OE_CTL &= CSC9270_OE;//init the output low
  if(CSC9270_DV_STA != 0)//have the dtmf signal
  {
     CSC9270_OE_CTL |= CSC9270_OE;//D0-D3 输出使能高电平有效
     __delay_cycles(20);
     val = dtmf_read();
     CSC9270_OE_CTL &= CSC9270_OE;
     return val;
  }
  return 0xff;
}

//**************************************
//外部中断0,下降沿触发（参考电话模块部分）
//
//
//*************************************
void phone_init(void)
{
  PORTB |= (1 << PB0); 
  DDRB &= ~(1 << DDB0);//捕获端口设置为输入
  
  EICRA = 0x02;//int0 下降沿触发
  EIMSK = 0x01;//int0 enable
}

#pragma vector = INT0_vect
__interrupt void into_for_phone_isr(void)
{
   static uint8 ucCount = 0x00;


    ucCount = ucCount + 1;

    if (2 == ucCount)      //ring is ringing   three times
    {
        g_bDetRingFst = 1;  //have phone  
        ucCount = 0;

    }


}
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
