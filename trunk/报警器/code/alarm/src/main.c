//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file	main.c
/// @brief	主程序入口
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2010-04-11                 
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////
#define _MAIN_C_
 
#include "def.h"
#include "main.h"
#include "key.h"
#include "decode.h"
#include "eeprom.h"
#include "phone.h"
#include "time.h"

STWORK gSysinfo;

static void init_devices(void);
static void decice_test(void);
//////////////////////////////////////////////////////////////////////////
///
///     main主函数
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
///     @note		不能删除
//////////////////////////////////////////////////////////////////////////
int main(void)
{
	init_devices();	//init devices
	timer0_init();	//初始化定时器
	decice_test();	//设备自我检测
	while(1)
	{
		key_process();//key process
		
		/*decode*/
		decode_process();
	//	watch_dog();//最后加上
	}
}

//////////////////////////////////////////////////////////////////////////
///
///     芯片及外围器件初始化
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
static void init_devices(void)
{
	phone_work_read(&gSysinfo);
	gSysinfo.is_key_open = KEY_CLOSE;//调正为键盘关闭
	//其他初始化看着办
	phone_init();//来电中断初始化
}
//////////////////////////////////////////////////////////////////////////
///
///     设备自我检测，所有LED闪烁加蜂鸣器鸣叫
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
static void decice_test(void)
{
	//所有LED闪烁几下
	//蜂鸣器和警铃叫几下
}
//////////////////////////////////////////////////////////////////////////
///
///     报警输出，蜂鸣器鸣叫
///		@author		xul	<gxuliang@gmail.com>          
///		@date		2010-04-11                 
//////////////////////////////////////////////////////////////////////////
void alarm_output(bool val)
{
	 //PD3
	//需考虑鸣响时间
}

/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
