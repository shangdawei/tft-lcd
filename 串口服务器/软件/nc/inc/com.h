/*
**com.h
*/
#ifndef _COM_H_
#define _COM_H_
#include <termios.h>
#include "conf.h"


#define TTY_DEV "/dev/ttyF"

typedef struct tagCOM_STA
{
    int id;
    int fd;
    int p_fd[2];    //�ܵ��������̼߳�ͨ�Ŵ�������
    UQWORD  issue[2];  //����
} COM_STA;

extern COM_STA g_com_status[MAX_NC_PORT];
extern void com_start(void);
extern void com_reset(int i);

#endif
/*=================================com.h end============================================*/


