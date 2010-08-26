//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   decode.h
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

#ifndef _DECODE_H
#define _DECODE_H

enum  {
  BUFANG1_CMD  = 1,//留守布防
  BUFANG2_CMD,      //部分布防
  CHEFANG_CMD,     //撤防
  ITC_CMD,         //紧急报警  
};

enum {
  REMOTE_ADDR  = 0xAAAA,//
  
};

extern void DecodeProcess(void);

#endif