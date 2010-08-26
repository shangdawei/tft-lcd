//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   decode.c
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
#include "decode.h"
#include "stm8s_itc.h"


static BYTE DecodeAddr, DecodeData;
//////////////////////////////////////////////////////////////////////////
///
///     ��ֵ���뻺�壬���ж��Ƿ���һָ�������ָ�����ȫ�ֱ�����
///     @param ch-������ַ���ret-������سɹ������ṩ��������
///     @return boolֵ��1-��ȡ��0-δ��ȡ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static BYTE DecodeRead(BYTE* ch)
{
  
  return 0;
}
//////////////////////////////////////////////////////////////////////////
///
///     RemoteData
///     @param data
///     @return boolֵ��1-��ȡ��0-δ��ȡ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
inline BYTE RemoteData(BYTE data)
{
  return data;
}
//////////////////////////////////////////////////////////////////////////
///
///     �Ѽ�ֵ���뻺�壬���ж��Ƿ���һָ�������ָ�����ȫ�ֱ�����
///     @param ch-������ַ���ret-������سɹ������ṩ��������
///     @return boolֵ��1-��ȡ��0-δ��ȡ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static BYTE DecodeAnalyze(BYTE *ch, BYTE *ret)
{
  DecodeAddr = ch[0];
  DecodeData = ch[1];
  switch(DecodeAddr)
  {
  case REMOTE_ADDR:
    *ret = RemoteData(DecodeData);
    return 1;
  default:
    break;
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
static void DecodeCmdDone(BYTE type)
{
  
}
//////////////////////////////////////////////////////////////////////////
///
///     DecodeProcess
///     @param ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
void DecodeProcess(void)
{
  BYTE ch[2], ret;
  if(DecodeRead(ch))
  {
    if(DecodeAnalyze(ch, &ret))
    {
      DecodeCmdDone(ret);
    }
  }
}


#pragma vector = ITC_IRQ_TIM1_CAPCOM
__interrupt void timer1_capt_isr(void)
{
  
  
}