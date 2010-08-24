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


#include "stm8s.h"
#include "def.h"
#include "device.h"
#include "decode.h"
#include "key.h"
#include "alarm.h"

//////////////////////////////////////////////////////////////////////////
///
///     main
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
int main( void )
{
  DeviceInit();
  
  while(1)
  {
    KeyProcess(); 
    DecodeProcess();
    AlarmInProcess();//有线防区处理
    //	watch_dog();//最后加上
  }
  
}
