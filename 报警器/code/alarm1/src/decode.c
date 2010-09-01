//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   decode.c
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
#include "decode.h"
#include "stm8s_itc.h"
#include "board.h"

static BYTE DecodeAddr, DecodeData;
//////////////////////////////////////////////////////////////////////////
///
///     把值存入缓冲，并判断是否是一指令结束，指令缓冲在全局变量中
///     @param ch-传入的字符，ret-如果返回成功，则提供命令类型
///     @return bool值，1-读取到0-未读取到
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
///     @return bool值，1-读取到0-未读取到
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
inline BYTE RemoteData(BYTE data)
{
  return data;
}
//////////////////////////////////////////////////////////////////////////
///
///     把键值存入缓冲，并判断是否是一指令结束，指令缓冲在全局变量中
///     @param ch-传入的字符，ret-如果返回成功，则提供命令类型
///     @return bool值，1-读取到0-未读取到
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
///     处理命令
///     @param type-命令类型
///     @return 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static void DecodeCmdDone(BYTE type)
{
  switch(type)
  {
  case BUFANG1_CMD://留守布防 
//    break;
  case BUFANG2_CMD://部分布防
//    break;
  case CHEFANG_CMD://撤防
    SetBF(type);
    break;
  case ITC_CMD://紧急报警  
    SetAlarm();
    break;
    
  default:
    break;
  }
}
//////////////////////////////////////////////////////////////////////////
///
///     DecodeProcess
///     @param 无
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