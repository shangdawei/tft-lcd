#ifndef	__NET_H__
#define	__NET_H__

#include "system.h"
#include "usocket.h"
#include "conf.h"
enum    tagNET_MODE
{
    MOD_NULL       = 0x00,
    MOD_TCP_UPD    = 0x01,
};

typedef struct tagNET_STA
{
    int id;
    int fd;
    int p_fd[2];    //管道，用于线程间通信传递命令
    int se_fd[MAX_SESSION];
    UQWORD  issue;  //流量
} NET_STA;

extern NET_STA g_net_status[MAX_NC_PORT];
extern int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len);

#endif

