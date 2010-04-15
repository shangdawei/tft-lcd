//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file	led.c
/// @brief	led�����غ���
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2009��09��15��                 
///
///
///     �޶�˵��������汾
//////////////////////////////////////////////////////////////////////////
#define _LED_C_

#include "def.h"
#include "led.h"
#include "time.h"

bool LED_Dn[3];
uint16 LED_Val;

//////////////////////////////////////////////////////////////////////////
///
///     led���IO��ʼ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
void init_led(void)
{
  LED_DAT_DIR |= LED_DAT;
  LED_CLR_DIR |= LED_CLR;
  LED_CLK_DIR |= LED_CLK;
  
  LED_CLR_CTL &= ~LED_CLR;//reset the 74HC164
  udelay(20);
  LED_CLR_CTL |= LED_CLR;
}

//////////////////////////////////////////////////////////////////////////
///
///     ����ʱ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
static void clk_h(void)
{
	LED_CLK_CTL |= LED_CLK;
}

//////////////////////////////////////////////////////////////////////////
///
///     ����ʱ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
static void clk_l(void)
{
	LED_CLK_CTL &= ~LED_CLK;
}

//////////////////////////////////////////////////////////////////////////
///
///     ��ʾָʾ�ƺ�led
///     @param	val 16λֵ��ʾ2������ܺ�2��ָʾ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
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
    udelay(20);
    clk_h();
    udelay(20);
    val = val << 1;
  }
}

//////////////////////////////////////////////////////////////////////////
///
///     ����Ҫ��ʾ��ֵ����ת��
///     @param	*p ��Ҫת����ֵ
///     @return	1-ת���ɹ���0-ת��ʧ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
///
///     ��2���������ʾ
///     @param	a ��һ������ܵ�ֵ
///     @param	ab�ڶ�������ܵ�ֵ
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
///
///     ��3��led����ʾ
///     @param	nu led��id��
///     @param	val 1-��ʾ������0-��ʾϨ��
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009��09��26��
//////////////////////////////////////////////////////////////////////////
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
