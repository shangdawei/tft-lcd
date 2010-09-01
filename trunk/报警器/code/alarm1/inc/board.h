//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   board.h
/// @brief  板卡配置
///
///
/// "$Id$"
/// @version    1.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010-09-01
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////

#ifndef _BOARD_H 
#define _BOARD_H

#include "def.h"


typedef struct _tagCmd_
{
  BYTE bf;//0x00-撤防 0x01-留守布防 0x02-部分布防  0x03-二者都布防
}BCmd;

typedef struct _tagBoard_
{
  DWORD id;    
  BYTE  bf_state;//低4位：布防状态 0x00-撤防 0x01-半留守布防，0x02-半部分布防，0x03-二者都半布防
                 //高4位：0x00-没有布防  0x01-留守布防  0x02-部分布防  0x03-二者都布防
  BYTE  al_flag;//紧急报警标志，0-否，1-是
}Board;


extern Board OldCard, NewCard;

extern void SetBF(BYTE type);
extern void SetAlarm(void);
#endif