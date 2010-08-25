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

//////////////////////////////////////////////////////////////////////////
///
///     main
///     @param ��
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
    AlarmInProcess();//���߷�������
    //	watch_dog();//������
  }
  
}
