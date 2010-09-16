//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   key.c
/// @brief  参数配置
///
///
/// "$Id$"
/// @version    1.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010－04－24
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "key.h"

//用来缓冲键值
static BYTE lsbuf[32]="";
const BYTE KeyVal[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
//////////////////////////////////////////////////////////////////////////
///
///     判断是否处于编程模式
///     @return bool值，1-是0-不是
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-25
//////////////////////////////////////////////////////////////////////////
static BYTE IsProgaMode(void)
{
   GPIOE->DDR &= ~0x01;//PE0 as 24LC04B_WP
   GPIOE->CR1 |= 0x01;//PE0 input with pull-up
   if(GPIOE->IDR & 0x01)
     return 1;//编程模式
   else 
     return 0;
}
//////////////////////////////////////////////////////////////////////////
///
///     轮询，读取键值，需要注意的是要区别在编程模式和非编程模式下轮询的按键数不一样
///     @param 获取的字符值
///     @return bool值，1-读取到0-未读取到
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static BYTE KeyRead(BYTE* ch)
{
  BYTE i;
  if(IsProgaMode() == 0)
    return 0;
  
  //先把状态清掉
  GPIOD->DDR |= GPIO_PIN_2;
  GPIOD->CR1 |= GPIO_PIN_2;
  
  GPIOC->DDR |= GPIO_PIN_3;
  GPIOC->CR1 |= GPIO_PIN_3;
  
  GPIOD->ODR &= ~GPIO_PIN_2;//PD2 as AB
  for(i = 0; i < 8; i++)
  {
    //PC3产生一个上升沿，可能要加延迟
    GPIOC->ODR &= ~GPIO_PIN_3;//PC3 as CLK
    GPIOC->ODR |= GPIO_PIN_3;//PC3 as CLK
  }
  
  GPIOD->CR1 &= ~0x18;//8P 9P 下拉
  GPIOD->ODR |= GPIO_PIN_2;
  GPIOC->ODR &= ~GPIO_PIN_3;//PC3 as CLK
  GPIOC->ODR |= GPIO_PIN_3;//PC3 as CLK
  if(GPIOD->IDR & 0x10)
  {
    *ch = KeyVal[0];
    return 1;
  }
  if(GPIOD->IDR & 0x08)
  {
    *ch = KeyVal[8];
    return 1;
  }
  
  GPIOD->ODR &= ~GPIO_PIN_2;//PD2 as AB
  for(i = 1; i < 8; i++)
  {
    GPIOC->ODR &= ~GPIO_PIN_3;//PC3 as CLK
    GPIOC->ODR |= GPIO_PIN_3;//PC3 as CLK
    if(GPIOD->IDR & 0x10)
    {
      *ch = KeyVal[i];
      return 1;
    }
    if(GPIOD->IDR & 0x08)
    {
      *ch = KeyVal[i+8];
      return 1;
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////
///
///     把键值存入缓冲，并判断是否是一指令结束，指令缓冲在全局变量中
///     @param ch-传入的字符，ret-如果返回成功，则提供命令类型
///     @return bool值，1-读取到0-未读取到
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
BYTE ls_cnt;
BYTE cmd_flag;

static BYTE KeyAnalyze(BYTE ch, BYTE *ret)
{
    BYTE i,j;
  BYTE key_value;
  
  if(IsProgaMode() == 0)
    return 0;
  key_value = ch;
  if(key_value != 0xFF)
  {
    lsbuf[ls_cnt++] = key_value;
  }
   
  if(!(ls_cnt < 32))
    ls_cnt = 0;
  
  if(lsbuf[0] != 0x0C || 0x0E)   //“储存”或“对码”键
    ls_cnt = 0;
    return 0;
  for(i = 2;i < 31;i++)
  {
    if((lsbuf[i] != 0x0C) && (lsbuf[i] != 0x0D) && (lsbuf[i] != 0x0F))
    {
      if(i == 30)
        return 0;
    }
    else
    {
      j = i;
      break;
    }  
  }
  
  
  if(cmd_flag)  
  {
    switch(lsbuf[0])
    {
    case 0x0C: 
      if(lsbuf[j] == 0x0C)
      {
        switch(lsbuf[j + 1])
        {
        case 0x00:
          * ret = 1;                   //*ret = 1 进入延时和退出延时时间设置
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 2;                   //*ret = 2 接警电话号码设置
          ls_cnt = 0;
          return 1;
        case 0x0A:
          * ret = 3;                   //*ret = 3 布防1、布防2防区设置
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      if(lsbuf[j] == 0x0F)            //“确认”键
      {
        switch(lsbuf[j - 1 ])
        {
        case 0x00:
          * ret = 4;                   //*ret = 4 进入延时和退出延时清除
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 5;                   //*ret = 5 接警电话号码清除
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      break;    
    case 0x0E:
      if(lsbuf[j] == 0x0D)            //“清除”键
      {
        switch(lsbuf[j - 1])
        {
        case 0:
          * ret = 6;                   //*ret = 6 遥控器学习码清除
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          * ret = 7;                   //*ret = 7 探测器或门磁学习码清除
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      if(lsbuf[j] == 0x0F)            //“确认”键
      {
        switch(lsbuf[j - 1])
        {
        case 0:
          * ret = 8;                   //*ret = 8 遥控器对码
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          * ret = 9;                   //*ret = 9 探测器或门磁对码
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      break;
    default:
      return 0;
    }  
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////
///
///     处理命令
///     @param type-命令类型
///     @return 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static void KeyCmdDone(BYTE type)
{
  
}
//////////////////////////////////////////////////////////////////////////
///
///     KeyProcess
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
void KeyProcess(void)
{
  BYTE ch, ret;
  if(KeyRead(&ch))
  {
    if(KeyAnalyze(ch, &ret))
    {
      KeyCmdDone(ret);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
///
///     KeyProcess
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
void KeyInit(void)
{
  GPIOD->DDR &= ~0x18;//PD4 as 8P, PD3 as 9P
}