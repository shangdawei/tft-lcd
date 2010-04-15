//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file	led.c
/// @brief	led输出相关函数
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2009年09月15日                 
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////
#define _LED_C_

#include "def.h"
#include "led.h"
#include "time.h"

bool LED_Dn[3];
uint16 LED_Val;

//////////////////////////////////////////////////////////////////////////
///
///     led相关IO初始化
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
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
///     拉高时钟
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
//////////////////////////////////////////////////////////////////////////
static void clk_h(void)
{
	LED_CLK_CTL |= LED_CLK;
}

//////////////////////////////////////////////////////////////////////////
///
///     拉低时钟
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
//////////////////////////////////////////////////////////////////////////
static void clk_l(void)
{
	LED_CLK_CTL &= ~LED_CLK;
}

//////////////////////////////////////////////////////////////////////////
///
///     显示指示灯和led
///     @param	val 16位值表示2个数码管和2个指示灯
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
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
///     对需要显示的值进行转换
///     @param	*p 需要转换的值
///     @return	1-转换成功，0-转换失败
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
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
///     对2个数码管显示
///     @param	a 第一个数码管的值
///     @param	ab第二个数码管的值
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
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
///     对3个led灯显示
///     @param	nu led的id号
///     @param	val 1-表示点亮，0-表示熄灭
///		@author xuliang<gxuliang@gmail.com>
///		@date	2009年09月26日
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
