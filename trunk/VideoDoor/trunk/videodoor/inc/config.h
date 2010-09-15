//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _CONFIG_H
#define _CONFIG_H

#include "globleinc.h"
#include "support.h"


typedef enum    tagCONF_TYPE
{
    TYPE_SYS   = 1,
    TYPE_NET,
}CONF_TYPE;

typedef	struct	tagCONFIG_SYS
{
    DWORD		id;
    DWORD       type;
    BYTE		hostname[64];
    BYTE		pos_info[64];
    BYTE        rsvd[32];
} CONFIG_SYS;


typedef	struct	tagCONFIG_NET
{
    DWORD       ip_addr;
    DWORD       netmask;
    DWORD       gateway;
    DWORD       dns[2];	 
    DWORD       cen_svr;//中心服务器地址
    WORD        cen_svr_port;//中心服务器地址端口
    BYTE        mac[6];
    BYTE        rsvd[32];
} CONFIG_NET;

typedef struct tagCONF_DATA
{
    CONFIG_SYS      con_sys;
    CONFIG_NET      con_net;
} CONF_DATA;

typedef union tagCONF_UNION
{
    CONFIG_SYS      con_sys;
    CONFIG_NET      con_net;
} CONF_UNION;


#endif


