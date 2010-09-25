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

/*#define  DELAYTIME_SET_CMD   1; //��ʱʱ������
#define  PHONENUM_SET_CMD    2; //�����绰��������
#define  DEFENDAREA_SET_CMD  3; //��������
#define  DELAYTIME_CLN_CMD   4; //��ʱʱ�����
#define  PHONENUM_CLN_CMD    5; //�����绰�������
#define  TELECONTROL_CLN_CMD 6; //ң����ѧϰ�����
#define  DETECTOR_CLN_CMD    7; //̽�������Ŵ�ѧϰ�����
#define  TELECONTROL_STY_CMD 8; //ң��������
#define  DETECTOR_STY_CMD    9; //̽�������ŴŶ���*/
static BYTE lsbuf[32]="";
const BYTE KeyVal[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
/*          ʵ�ʰ��� 1  2  3  4  5  6  7  8  9  0   *    #  �洢  ��� ����  ȷ��*/
/*struct studycode
{
  BYTE DEFENDAREA_BUF1[12];
  BYTE DEFENDAREA_BUF2[12];
  BYTE DEFENDAREA_BUF3[12];
}def_area1,def_area2,def_area3,def_area4,def_area5,def_area6,def_area7,def_area8;*/
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
    if(key_value == 0x0F)               //�ж�ָ�������־��ȷ�ϡ�
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
    cmd_flag = 0;                //����ָ����������ָ���־λ
    //switch(lsbuf[0])
    if(lsbuf[0] == 0x0C)
    {
   // case 0x0C:                   //�����桱ָ��ͷ����1����ֵΪ�����桱
      switch(lsbuf[i - 1])
      {
      case 0x00:               //������2����ֵΪ��0��
        * ret = 1;                   //*ret = 1 ������ʱ���˳���ʱʱ������
        ls_cnt = 0;
        return 1;
        
      case 0x01:               //������2����ֵΪ��1��
      case 0x02:               //������2����ֵΪ��2��
      case 0x03:               //������2����ֵΪ��3��
      case 0x04:               //������2����ֵΪ��4��
      case 0x05:               //������2����ֵΪ��5��
      case 0x06:               //������2����ֵΪ��6��
      case 0x07:               //������2����ֵΪ��7��
        * ret = 2;                   //*ret = 2 �Ӿ��绰��������
        ls_cnt = 0;
        return 1;
        
      case 0x0A:               //������2����ֵΪ��*��
        * ret = 3;                   //*ret = 3 ����1������2��������
        ls_cnt = 0;
        return 1;
        
      case 0x0D:               //������2����ֵΪ�������
        if(lsbuf[1] == 0x00)
        {
          * ret = 4;                 //*ret = 4,ɾ�������ý�����ʱ���˳���ʱʱ��
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x08))
        {
          * ret = 5;                 //*ret = 5,ɾ�������õı����绰����
          ls_cnt = 0;
          return 1;
        }
        break;
        /* switch(lsbuf[1])              //switch�ṹ
        {
        case 0x00:
          * ret = 4;                 //*ret = 4,ɾ�������ý�����ʱ���˳���ʱʱ��
          ls_cnt = 0;
          return 1;
          
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          * ret = 5;                 //*ret = 5,ɾ�������õı����绰����
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
    //case 0x0E:                   //�����롱ָ��ͷ����1����ֵΪ�����롱
    else if(lsbuf[0] == 0x0E)  
    {
      switch(lsbuf[i - 1])
      {
      case 0x0D:               //������2����ֵΪ�������
        if(lsbuf[1] == 0x00)
        {
          * ret = 6;                 //*ret = 6 ң����ѧϰ�����
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x09))
        {
          * ret = 7;                 //*ret = 7 ̽�������Ŵ�ѧϰ�����
          ls_cnt = 0;
          return 1;
        }
        break;
        /*  switch(lsbuf[1])        ////switch jiegou
        {
        case 0:
          * ret = 6;                 //*ret = 6 ң����ѧϰ�����
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
          * ret = 7;                 //*ret = 7 ̽�������Ŵ�ѧϰ�����
          ls_cnt = 0;
          return 1;
          
        default:
          break;
        }
        break;*/
      case 0x0E:               //������2����ֵΪ�����롱
        if(lsbuf[1] == 0x00)
        {
          * ret = 8;                   //*ret = 8 ң��������
          ls_cnt = 0;
          return 1;
        }
        else if((lsbuf[1] > 0x00)&&(lsbuf[1] < 0x09))
        {
          * ret = 9;                   //*ret = 9 ̽�������ŴŶ���
          ls_cnt = 0;
          return 1;
        }
        break;
        /*switch(lsbuf[1])            ////switch jiegou
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
///     ��������
///     @param type-��������
///     @return ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
/////////////////////////////////////////////////////////////////////////
static void KeyCmdDone(BYTE type)
{
  switch(type)
  {
  case 1: //��ʱʱ������
    break;
  case 2:   //�����绰��������
    break;
  case 3:  //��������
    break;
  case 4:  //��ʱʱ�����
    break;
  case 5:   //�����绰�������
    break;
  case 6: //ң����ѧϰ�����
    break;
  case 7: //̽�������Ŵ�ѧϰ�����
    break;
  case 8: //ң��������
    break;
  case 9:    //̽�������ŴŶ���
    break;
  default:
    break;
  }
}
/////////////////////////////////////////////////////////////////////////
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