//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   config.c
/// @brief  参数配置
///
///
///
/// @version    2.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010－04－24
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "decode.h"


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