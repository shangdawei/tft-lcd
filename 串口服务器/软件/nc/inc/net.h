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

enum	tagNET_PROTOCOL
{
    P_NULL,
    P_TCP_CLI,
    P_TCP_SRV,
    P_UDP,
};

//Net Connection information
typedef struct tagNET_CONN_INF
{
    int idx;
    int conn_idx;		            //!< Index of connection
    int client_conn;	            //!< client socket descriptor
    BYTE client_type;	            //!< client type
    BYTE file_type;		            //!< file type, 0 - picture, 1 - video, 2 -- break-point continue
    BYTE idle;	            		//!< idle time count
    char clientip[16];
} NET_CONN_INF, *pNET_CONN_INF;

/*for every port*/
typedef struct tagNET_STA
{
    int id;
    int fd[MAX_SESSION];
    int p_fd[2];    //管道，用于线程间通信传递命令
//    int se_fd[MAX_SESSION];
    NET_CONN_INF conn_status[MAX_SESSION];
    UQWORD  issue;  //流量
} NET_STA;


extern NET_STA g_net_status[MAX_NC_PORT];
extern int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len);
extern void net_reset(int i);
#endif

