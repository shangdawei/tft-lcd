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

    TYPE_ALL,
} CONF_TYPE;


typedef enum    tagDEV_TYPE
{
    OUT_TP = 1,
    IN_TP,
} DEV_TYPE;

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

/////////////////////////////////////////////////
extern CONF_DATA   g_conf_info;
void config_init(void);
int sys_config_load(CONF_TYPE load_type);

#endif


