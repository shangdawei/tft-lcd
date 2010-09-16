//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   key.c
/// @brief  ��������
///
///
/// "$Id$"
/// @version    1.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010��04��24
///
///
///     �޶�˵��������汾
//////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "key.h"

//���������ֵ
static BYTE lsbuf[32]="";
const BYTE KeyVal[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
//////////////////////////////////////////////////////////////////////////
///
///     �ж��Ƿ��ڱ��ģʽ
///     @return boolֵ��1-��0-����
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-25
//////////////////////////////////////////////////////////////////////////
static BYTE IsProgaMode(void)
{
   GPIOE->DDR &= ~0x01;//PE0 as 24LC04B_WP
   GPIOE->CR1 |= 0x01;//PE0 input with pull-up
   if(GPIOE->IDR & 0x01)
     return 1;//���ģʽ
   else 
     return 0;
}
//////////////////////////////////////////////////////////////////////////
///
///     ��ѯ����ȡ��ֵ����Ҫע�����Ҫ�����ڱ��ģʽ�ͷǱ��ģʽ����ѯ�İ�������һ��
///     @param ��ȡ���ַ�ֵ
///     @return boolֵ��1-��ȡ��0-δ��ȡ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static BYTE KeyRead(BYTE* ch)
{
  BYTE i;
  if(IsProgaMode() == 0)
    return 0;
  
  //�Ȱ�״̬���
  GPIOD->DDR |= GPIO_PIN_2;
  GPIOD->CR1 |= GPIO_PIN_2;
  
  GPIOC->DDR |= GPIO_PIN_3;
  GPIOC->CR1 |= GPIO_PIN_3;
  
  GPIOD->ODR &= ~GPIO_PIN_2;//PD2 as AB
  for(i = 0; i < 8; i++)
  {
    //PC3����һ�������أ�����Ҫ���ӳ�
    GPIOC->ODR &= ~GPIO_PIN_3;//PC3 as CLK
    GPIOC->ODR |= GPIO_PIN_3;//PC3 as CLK
  }
  
  GPIOD->CR1 &= ~0x18;//8P 9P ����
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
///     �Ѽ�ֵ���뻺�壬���ж��Ƿ���һָ�������ָ�����ȫ�ֱ�����
///     @param ch-������ַ���ret-������سɹ������ṩ��������
///     @return boolֵ��1-��ȡ��0-δ��ȡ��
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
  
  if(lsbuf[0] != 0x0C || 0x0E)   //�����桱�򡰶��롱��
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
          * ret = 1;                   //*ret = 1 ������ʱ���˳���ʱʱ������
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 2;                   //*ret = 2 �Ӿ��绰��������
          ls_cnt = 0;
          return 1;
        case 0x0A:
          * ret = 3;                   //*ret = 3 ����1������2��������
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      if(lsbuf[j] == 0x0F)            //��ȷ�ϡ���
      {
        switch(lsbuf[j - 1 ])
        {
        case 0x00:
          * ret = 4;                   //*ret = 4 ������ʱ���˳���ʱ���
          ls_cnt = 0;
          return 1;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 5;                   //*ret = 5 �Ӿ��绰�������
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      break;    
    case 0x0E:
      if(lsbuf[j] == 0x0D)            //���������
      {
        switch(lsbuf[j - 1])
        {
        case 0:
          * ret = 6;                   //*ret = 6 ң����ѧϰ�����
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
          * ret = 7;                   //*ret = 7 ̽�������Ŵ�ѧϰ�����
          ls_cnt = 0;
          return 1;
        default:
          break;
        }
      }
      if(lsbuf[j] == 0x0F)            //��ȷ�ϡ���
      {
        switch(lsbuf[j - 1])
        {
        case 0:
          * ret = 8;                   //*ret = 8 ң��������
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
          * ret = 9;                   //*ret = 9 ̽�������ŴŶ���
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
///     ��������
///     @param type-��������
///     @return ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static void KeyCmdDone(BYTE type)
{
  
}
//////////////////////////////////////////////////////////////////////////
///
///     KeyProcess
///     @param ��
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
///     @param ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
void KeyInit(void)
{
  GPIOD->DDR &= ~0x18;//PD4 as 8P, PD3 as 9P
}