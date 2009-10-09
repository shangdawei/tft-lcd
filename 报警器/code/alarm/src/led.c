/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: led.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/15  || Create this file
**  ******************************************************************************************
*/
#define _LED_C_

#include "def.h"
#include "led.h"

bool LED_Dn[3];
uint16 LED_Val;

void init_led(void)
{
  LED_DAT_DIR |= LED_DAT;
  LED_CLR_DIR |= LED_CLR;
  LED_CLK_DIR |= LED_CLK;
  
  LED_CLR_CTL &= ~LED_CLR;//reset the 74HC164
  __delay_cycles(20);
  LED_CLR_CTL |= LED_CLR;
}


static void clk_h(void)
{
	LED_CLK_CTL |= LED_CLK;
}

static void clk_l(void)
{
	LED_CLK_CTL &= ~LED_CLK;
}
void put_led(uint16 val)
{
  uint8 i;
  init_led();
  
  for(i = 0; i < 16; i++)
  {
    if((val & 0x80) == 0x80)
    {
      LED_DAT_CTL |= LED_DAT; 
    }
    else
    {
      LED_DAT_CTL &= ~LED_DAT;
    }
    clk_l();
    __delay_cycles(20);
    clk_h();
    __delay_cycles(20);
    val = val << 1;
  }
}


bool judge_led_val(uint8* p)
{
   switch(*p)
  {
    case 'F':
      *p = 10;
      break;
    case 'S':
      *p = 11;
      break;
    case 'b':
      *p = 12;
      break;
    default:
      break;
  }
  if(*p > 12)
    return 0; 
  else
    return 1;
}
/*  $Function   :   show_digled
==  ==============================================================================================
==  Description :   œ‘ æLED
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/26  || Create this function
==  ===============================================================================================
*/
void show_digled(uint8 a,uint8 b)
{
  uint16 val;
  if(judge_led_val(&a) == 0)
    return ;
  if(judge_led_val(&b) == 0)
    return ;
  
  LED_Val = (dig2[b] << 7 | dig1[a]) & 0x3F;
  val = LED_Val | (LED_Dn[D3] << 15) | (LED_Dn[D2] << 14);
  put_led(val);
}


void show_led(uint8 nu, bool val)
{
  uint16 ret;
  if(nu < 2)
  {
     LED_Dn[nu] = val;
     ret = LED_Val | (LED_Dn[D3] << 15) | (LED_Dn[D2] << 14);
     put_led(ret);
     return ;
  }
  else if(nu == 2)
  {
     LED_D4_DIR |= LED_D4;
     if(val == 1)
       LED_D4_CTL |= LED_D4;
     else
       LED_D4_CTL &= ~LED_D4;
  }
}

/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
