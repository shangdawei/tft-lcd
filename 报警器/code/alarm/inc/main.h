/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: main.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/24  || Create this file
**  ******************************************************************************************
*/

#ifndef _MAIN_H_
#define _MAIN_H_
#include "def.h"

#ifndef _MAIN_C_
#define GLOBLE_MAIN extern
#else
#define GLOBLE_MAIN
#endif

#define ALARM_ON  1
#define ALARM_OFF  0



typedef struct {
  bool alarm_md;//����-1/����-0
  bool bal_md;//���߱������� 1-������0-�ر�
  bool sery_md;//���뱣������ 1-������0-�ر�
  bool al_sd_md;//0-����������1-��������
  bool re_al_sd_md;//ң�ؾ��Ű��� 1-������0-�ر�
  uint8 beep_tm;//��������ʱ��
  uint8 al_del_tm;//������ʱʱ��
  uint8 re_ring_tm;//Զ�̲��������������
  bool l_al;//���߷��� 1-������0-�ر�
  
  uint16 password;
  uint16 localID;//����ID
  bool istel_set;//�绰�����Ƿ�����
} STWORK;

#define BAL_MD  1
#define SERY_MD 2
#define AL_SD_MD  3
#define RE_AL_SD_MD 4
#define BEEP_TM_MD  5
#define AL_DEL_TM_MD  6
#define RE_RING_TM_MD 7
#define L_AL_MD   8
#define REC_MD    0x10

GLOBLE_MAIN STWORK gSysinfo;


GLOBLE_MAIN void alarm_output(bool val);

#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
