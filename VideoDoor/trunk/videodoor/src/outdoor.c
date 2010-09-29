//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "outdoor.h"
#include "net.h"
#include "trd.h"

NET_INFO gOutNet;

static void outDoorProcess(void);

//////////////////////////////////////////////////////////////////////////
///
///     outDoorInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
void outDoorInit(void)
{
    TRD_t trd_outdoor;
    memset(&gOutNet, 0, sizeof(NET_INFO));
    
    ///此处监听网络端口，用于传输多媒体数据
    net_init(gOutNet.conn, gOutNet.serv);
    trd_create(&trd_outdoor, (void *)&outDoorProcess);
}

//////////////////////////////////////////////////////////////////////////
///
///     doorOpen
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
int doorOpen(void)
{
    int ret = 0;


    return ret;

}


//////////////////////////////////////////////////////////////////////////
///
///     outDoorProcess
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-28
//////////////////////////////////////////////////////////////////////////
static void outDoorProcess(void)
{

    while(1)
    {
        net_process(&gOutNet);
    }
}

