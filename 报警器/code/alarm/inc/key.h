/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: key.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#ifndef _KEY_H_
#define _KEY_H_
#include "def.h"

#ifndef _KEY_C_
#define GOLBLE_KEY  extern
#else
#define GOLBLE_KEY
#endif

#define MAX_PHONE_NUM 6

#define ROW_PIN (1<<PC1 | 1<<PC2 | 1<<PC3 | 1<<PC4 | 1<<PC5)
#define KEY_PIN PORTC

const uint8 keyVal[] = {1,2,3,'F',4,5,6,'G',7,8,9,'H','*',0,'#','I','A','B','C'};

typedef struct {
  uint8 mode;//����ģʽ���Ը���������
  uint8 serial;
  uint8 val;
  uint8 phone_num[16];
  uint8 old_passwd;
  uint8 new_passwd;
  uint8 id;
} KER_PAR;

//����ָ�뺯��
typedef struct {
	char	*ParseStr;
	void	(*ParseProc)(KER_PAR* );
} PAR_TAB;


//F1=F=���
//F2=G=����
//F3=H=¼��
//F4=I=���
#define REC_LWORD   "H^H"     //¼������
#define SET_PNUM    "G^*^G"   //���õ绰����
#define BAL_OFF     "F1*0F"   //���߱������ܹر�
#define BAL_ON      "F1*1F"   //���߱������ܿ���
#define SERY_OFF    "F2*0F"   //�ر����뱣������
#define SERY_ON     "F2*1F"   //�������뱣������
#define AL_SD_OFF   "F3*0F"   //��������
#define AL_SD_ON    "F3*1F"   //��������
#define RE_AL_SD_OFF "F4*0F"  //ң�ؾ��Ű����Ĺر�
#define RE_AL_SD_ON  "F4*1F"  //ң�ؾ��Ű����Ĵ�
#define BEEP_TIME    "F5*??F" //��������ʱ��
#define AL_DELAY_TM  "F6*??F" //������ʱʱ��
#define RE_RING_TM   "F7*?F"  //Զ�̲��������������
#define WL_STATE     "F8*^?F" //�޸����߷�����״̬
#define L_AL_OFF     "G0*0G"  //���߷����ر�
#define L_AL_ON      "G0*1G"  //���߷�������
#define PASD_SET     "G7*^^G" //�޸Ĳ������������
#define BD_ID_SET    "G9*^G"  //����ID������
#define BD_RESET     "I^^I"   //�����ܸ�λ


GOLBLE_KEY  void key_rec_set(KER_PAR*);
GOLBLE_KEY  void key_phone_num_set(KER_PAR*);
GOLBLE_KEY void key_bal_set(KER_PAR* p);
#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

