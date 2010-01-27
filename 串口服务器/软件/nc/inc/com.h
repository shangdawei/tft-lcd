/*
**com.h
*/
#ifndef _COM_H_
#define _COM_H_
#include "conf.h"


#define TTY_DEV "/dev/ttyF"

typedef struct tagCOM_STA
{
    int id;
    int fd;
    int p_fd[2];    //管道，用于线程间通信传递命令
    UQWORD  issue;  //流量
} COM_STA;

extern COM_STA g_com_status[MAX_NC_PORT];
extern void com_start(void);

#endif
/*=================================com.h end============================================*/


