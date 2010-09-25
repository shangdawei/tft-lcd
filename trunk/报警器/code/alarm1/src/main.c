//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   main.c
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
#include "device.h"
#include "decode.h"
#include "key.h"
#include "alarm.h"
#include "led.h"

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

void delayms(WORD ms)
{
    WORD i;
    while(ms--)
    {
        for(i = 0; i < 16000; i++)
          __no_operation();
    }
}
//////////////////////////////////////////////////////////////////////////
///
///     main
///     @param ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
int main( void )
{
  BYTE i;
  /* Configures clocks */
  CLK_Configuration();
  
  DeviceInit();
  
 /* while(1)
  {
    for(i = 0; i < 16; i++)
    {
      LedOn(0x0001 << i);
      delayms(50);
    }
    LedOff(0xffff);
    delayms(50);
  }*/
  
  while(1)
  {
    KeyProcess(); 
    DecodeProcess();
    AlarmInProcess();//���߷�������
    //	watch_dog();//������
  }
 
}
