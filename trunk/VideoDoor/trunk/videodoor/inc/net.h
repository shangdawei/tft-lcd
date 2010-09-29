//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _NET_H
#define _NET_H

#include "globleinc.h"
#include "support.h"

#define MAX_TCP_CONN    1
#define TIMEOUT_CNT 5000


typedef struct tagNET_CONN_INFO
{
    int conn_idx;		            //!< Index of connection
    int client_conn;	            //!< client socket descriptor
    BYTE client_type;	            //!< client type
    BYTE file_type;		            //!< file type, 0 - picture, 1 - video, 2 -- break-point continue
    BYTE idle;	            		//!< idle time count
    char clientip[16];
} NET_CONN_INFO, *pNET_CONN_INFO;

typedef struct  tagNET_SERV_INFO
{
    int  fd;
    WORD port;

}NET_SERV_INFO, *pNET_SERV_INFO;


typedef struct  tagNET_INFO
{
    NET_CONN_INFO* conn;
    NET_SERV_INFO* serv;
}NET_INFO;

//////////////////////////////////
void net_init(NET_CONN_INFO *conn, NET_SERV_INFO *serv);
void net_process(void* arg);

#endif

