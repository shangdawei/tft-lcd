//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   decode.h
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

#ifndef _DECODE_H
#define _DECODE_H

enum  {
  BUFANG1_CMD  = 1,//���ز���
  BUFANG2_CMD,      //���ֲ���
  CHEFANG_CMD,     //����
  ITC_CMD,         //��������  
};

enum {
  REMOTE_ADDR  = 0xAAAA,//
  
};

extern void DecodeProcess(void);

#endif