//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   device.c
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

#include "device.h"
#include "key.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////
///
///     中断初始化
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
void IrqInit(void)
{
  
}
//////////////////////////////////////////////////////////////////////////
///
///     DeviceInit
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
void DeviceInit(void)
{
  KeyInit();
  LedInit();
  IrqInit();
}