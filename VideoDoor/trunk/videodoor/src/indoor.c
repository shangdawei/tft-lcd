//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "indoor.h"
#include "config.h"
#include "trd.h"
#include "net.h"

NET_INFO gInNet;

static void inDoorProcess(void);

//////////////////////////////////////////////////////////////////////////
///
///     inDoorInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-28
//////////////////////////////////////////////////////////////////////////
void inDoorInit(void)
{
    TRD_t trd_indoor;
    
    memset(&gInNet, 0, sizeof(NET_INFO));
    
    ///此处监听网络端口，用于传输消息
    net_init(gInNet.conn, gInNet.serv);
    trd_create(&trd_indoor, (void *)&inDoorProcess);
}

//////////////////////////////////////////////////////////////////////////
///
///     net_svr_proc
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
int doorAsk(NET_CONN_INFO *conn_info)
{
    int ret = 0;


    return ret;
}

//////////////////////////////////////////////////////////////////////////
///
///     inDoorProcess
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-28
//////////////////////////////////////////////////////////////////////////
static void inDoorProcess(void)
{

    while(1)
    {
        net_process(&gInNet);
    }
}


