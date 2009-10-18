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



typedef struct {
  uint8 mode;//����ģʽ���Ը���������
  uint8 serial;
  uint8 val;
  uint8 phone_num[16];
  uint16 old_passwd;
  uint16 new_passwd;
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
#define REC_LWORD   "H^H\0"     //¼������
#define SET_PNUM    "G^*^G\0"   //���õ绰����
#define BAL_OFF     "F1*0F\0"   //���߱������ܹر�
#define BAL_ON      "F1*1F\0"   //���߱������ܿ���
#define SERY_OFF    "F2*0F\0"   //�ر����뱣������
#define SERY_ON     "F2*1F\0"   //�������뱣������
#define AL_SD_OFF   "F3*0F\0"   //��������
#define AL_SD_ON    "F3*1F\0"   //��������
#define RE_AL_SD_OFF "F4*0\0F"  //ң�ؾ��Ű����Ĺر�
#define RE_AL_SD_ON  "F4*1F\0"  //ң�ؾ��Ű����Ĵ�
#define BEEP_TIME    "F5*??F\0" //��������ʱ��
#define AL_DELAY_TM  "F6*??F\0" //������ʱʱ��
#define RE_RING_TM   "F7*?F\0"  //Զ�̲��������������
#define WL_STATE     "F8*^?F\0" //�޸����߷�����״̬
#define L_AL_OFF     "G0*0G\0"  //���߷����ر�
#define L_AL_ON      "G0*1G\0"  //���߷�������
#define PASD_SET     "G7*^^G\0" //�޸Ĳ������������
#define BD_ID_SET    "G9*^G\0"  //����ID������
#define BD_RESET     "I^^I\0"   //�����ܸ�λ


GOLBLE_KEY uint8 key_read(void);
GOLBLE_KEY  void key_rec_set(uint16);
GOLBLE_KEY  void key_phone_num_set(char*);
//GOLBLE_KEY void key_bal_set(KER_PAR* p);
GOLBLE_KEY void general_par_set(KER_PAR* p);
GOLBLE_KEY void key_analyse(char *p, uint8 num);
GOLBLE_KEY void key_process(void);
#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

