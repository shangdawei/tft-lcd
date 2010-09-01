//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   main.c
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
#include "device.h"
#include "decode.h"
#include "key.h"
#include "alarm.h"

/**
  ******************************************************************************
  * @brief Configures clocks
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void CLK_Configuration(void)
{

  /* Fmaster = 16MHz */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

}

//////////////////////////////////////////////////////////////////////////
///
///     main
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
int main( void )
{
  /* Configures clocks */
  CLK_Configuration();
  
  DeviceInit();
  
  while(1)
  {
    KeyProcess(); 
    DecodeProcess();
    AlarmInProcess();//有线防区处理
    //	watch_dog();//最后加上
  }
  
}
