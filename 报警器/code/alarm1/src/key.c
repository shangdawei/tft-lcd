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

/*#define  DELAYTIME_SET_CMD   1; //延时时间设置
#define  PHONENUM_SET_CMD    2; //报警电话号码设置
#define  DEFENDAREA_SET_CMD  3; //防区设置
#define  DELAYTIME_CLN_CMD   4; //延时时间清除
#define  PHONENUM_CLN_CMD    5; //报警电话号码清除
#define  TELECONTROL_CLN_CMD 6; //遥控器学习码清除
#define  DETECTOR_CLN_CMD    7; //探测器和门磁学习码清除
#define  TELECONTROL_STY_CMD 8; //遥控器对码
#define  DETECTOR_STY_CMD    9; //探测器和门磁对码*/
static BYTE lsbuf[32]="";
const BYTE KeyVal[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
/*          实际按键 1  2  3  4  5  6  7  8  9  0   *    #  存储  清除 对码  确认*/
/*struct studycode
{
  BYTE DEFENDAREA_BUF1[12];
  BYTE DEFENDAREA_BUF2[12];
  BYTE DEFENDAREA_BUF3[12];
}def_area1,def_area2,def_area3,def_area4,def_area5,def_area6,def_area7,def_area8;*/
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
BYTE cmd_flag = 0;

static BYTE KeyAnalyze(BYTE ch, BYTE *ret)
{
  BYTE i;
  BYTE key_value;
  
  if(IsProgaMode() == 0)
    return 0;
  key_value = ch;
  if(key_value != 0xFF)
  {
    lsbuf[ls_cnt++] = key_value;
    if(key_value == 0x0F)               //判断指令结束标志“确认”
    {
      cmd_flag = 1;
      i = ls_cnt - 1;
    }
  }
  if(!cmd_flag)
  {
    return 0;
  }
  else  
  {
    cmd_flag = 0;                //进入指令分析后清除指令标志位
    //switch(lsbuf[0])
    if(lsbuf[0] == 0x0C)
    {
   // case 0x0C:                   //“储存”指令头；第1个键值为“储存”
      switch(lsbuf[i - 1])
      {
      case 0x00:               //倒数第2个键值为“0”
        * ret = 1;                   //*ret = 1 进入延时和退出延时时间设置
        ls_cnt = 0;
        return 1;
        
      case 0x01:               //倒数第2个键值为“1”
      case 0x02:               //倒数第2个键值为“2”
      case 0x03:               //倒数第2个键值为“3”
      case 0x04:               //倒数第2个键值为“4”
      case 0x05:               //倒数第2个键值为“5”
      case 0x06:               //倒数第2个键值为“6”
      case 0x07:               //倒数第2个键值为“7”
        * ret = 2;                   //*ret = 2 接警电话号码设置
        ls_cnt = 0;
        return 1;
        
      case 0x0A:               //倒数第2个键值为“*”
        * ret = 3;                   //*ret = 3 布防1、布防2防区设置
        ls_cnt = 0;
        return 1;
        
      case 0x0D:               //倒数第2个键值为“清除”
        if(lsbuf[1] == 0x00)
        {
          * ret = 4;                 //*ret = 4,删除已设置进入延时和退出延时时间
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x08))
        {
          * ret = 5;                 //*ret = 5,删除已设置的报警电话号码
          ls_cnt = 0;
          return 1;
        }
        break;
        /* switch(lsbuf[1])              //switch结构
        {
        case 0x00:
          * ret = 4;                 //*ret = 4,删除已设置进入延时和退出延时时间
          ls_cnt = 0;
          return 1;
          
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 5;                 //*ret = 5,删除已设置的报警电话号码
          ls_cnt = 0;
          return 1;
          
        default:
          break;
        }*/
      default:
        break;
      }
    }
    //  break;
    //case 0x0E:                   //“对码”指令头；第1个键值为“对码”
    else if(lsbuf[0] == 0x0E)  
    {
      switch(lsbuf[i - 1])
      {
      case 0x0D:               //倒数第2个键值为“清除”
        if(lsbuf[1] == 0x00)
        {
          * ret = 6;                 //*ret = 6 遥控器学习码清除
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x09))
        {
          * ret = 7;                 //*ret = 7 探测器或门磁学习码清除
          ls_cnt = 0;
          return 1;
        }
        break;
        /*  switch(lsbuf[1])        ////switch jiegou
        {
        case 0:
          * ret = 6;                 //*ret = 6 遥控器学习码清除
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
          * ret = 7;                 //*ret = 7 探测器或门磁学习码清除
          ls_cnt = 0;
          return 1;
          
        default:
          break;
        }
        break;*/
      case 0x0E:               //倒数第2个键值为“对码”
        if(lsbuf[1] == 0x00)
        {
          * ret = 8;                   //*ret = 8 遥控器对码
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x09))
        {
          * ret = 9;                   //*ret = 9 探测器或门磁对码
          ls_cnt = 0;
          return 1;
        }
        break;
        /*switch(lsbuf[1])            ////switch jiegou
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
        break;*/
      default:
        break;
      }
      //break;
    //default:
     // return 0;
    }  
  return 0;
  }
}
//////////////////////////////////////////////////////////////////////////
///
///     处理命令
///     @param type-命令类型
///     @return 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
/////////////////////////////////////////////////////////////////////////
static void KeyCmdDone(BYTE type)
{
  switch(type)
  {
  case 1: //延时时间设置
    break;
  case 2:   //报警电话号码设置
    break;
  case 3:  //防区设置
    break;
  case 4:  //延时时间清除
    break;
  case 5:   //报警电话号码清除
    break;
  case 6: //遥控器学习码清除
    break;
  case 7: //探测器和门磁学习码清除
    break;
  case 8: //遥控器对码
    break;
  case 9:    //探测器和门磁对码
    break;
  default:
    break;
  }
}
/////////////////////////////////////////////////////////////////////////
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