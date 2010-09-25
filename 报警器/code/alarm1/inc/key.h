//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   key.h
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

#ifndef _KEY_H
#define _KEY_H

#include "def.h"
//////////////////////////////////////////////////////////////////////////
///
///     KeyProcess
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-23
//////////////////////////////////////////////////////////////////////////
extern void KeyProcess(void);
//////////////////////////////////////////////////////////////////////////
///
///     KeyInit
///     @param 无
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
extern void KeyInit(void);
/*enum  {
  DELAYTIME_SET_CMD = 1,  //延时时间设置
  PHONENUM_SET_CMD,    //报警电话号码设置
  DEFENDAREA_SET_CMD,  //防区设置
  DELAYTIME_CLN_CMD,   //延时时间清除
  PHONENUM_CLN_CMD,    //报警电话号码清除
  TELECONTROL_CLN_CMD, //遥控器学习码清除
  DETECTOR_CLN_CMD,    //探测器和门磁学习码清除
  TELECONTROL_STY_CMD, //遥控器对码
  DETECTOR_STY_CMD,    //探测器和门磁对码
};*/
/*#define  DELAYTIME_SET_CMD   1; //延时时间设置
#define  PHONENUM_SET_CMD    2; //报警电话号码设置
#define  DEFENDAREA_SET_CMD  3; //防区设置
#define  DELAYTIME_CLN_CMD   4; //延时时间清除
#define  PHONENUM_CLN_CMD    5; //报警电话号码清除
#define  TELECONTROL_CLN_CMD 6; //遥控器学习码清除
#define  DETECTOR_CLN_CMD    7; //探测器和门磁学习码清除
#define  TELECONTROL_STY_CMD 8; //遥控器对码
#define  DETECTOR_STY_CMD    9; //探测器和门磁对码*/


#endif