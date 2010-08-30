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
static BYTE KeyAnalyze(BYTE ch, BYTE *ret)
{
  
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