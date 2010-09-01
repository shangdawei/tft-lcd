//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   board.h
/// @brief  �忨����
///
///
/// "$Id$"
/// @version    1.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010-09-01
///
///
///     �޶�˵��������汾
//////////////////////////////////////////////////////////////////////////

#ifndef _BOARD_H 
#define _BOARD_H

#include "def.h"


typedef struct _tagCmd_
{
  BYTE bf;//0x00-���� 0x01-���ز��� 0x02-���ֲ���  0x03-���߶�����
}BCmd;

typedef struct _tagBoard_
{
  DWORD id;    
  BYTE  bf_state;//��4λ������״̬ 0x00-���� 0x01-�����ز�����0x02-�벿�ֲ�����0x03-���߶��벼��
                 //��4λ��0x00-û�в���  0x01-���ز���  0x02-���ֲ���  0x03-���߶�����
  BYTE  al_flag;//����������־��0-��1-��
}Board;


extern Board OldCard, NewCard;

extern void SetBF(BYTE type);
extern void SetAlarm(void);
#endif