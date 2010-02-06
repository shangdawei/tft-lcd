#ifndef __CONF_H__
#define	__CONF_H__
#include "system.h"


#define	DEV_VER "1.01"
#define	MAX_NC_PORT	16
#define	MAX_SESSION	6

enum CONF_OPT
{
    CONF_USR_SET = 0x00,					//!< User set configure
    CONF_DFT_SET,							//!< Default set configure
};
enum CONF_LIST
{
    CONF_ALL         = 0x00,                //!< All configure
    CONF_VER         = 0x01,                //!< Picture capture configure
    CONF_SYS		 = 0x02,
    CONF_NET         = 0x03,                //!< Video Record configure
    CONF_NC      	 = 0x04,                //!< Speed measure configure
    CONF_COM		 = 0x05,
    CONF_WKNET		 = 0x06,
    //!< Configure types count
}; /* CONF_LIST end */
//Configuration Version format: xx.xx.xx, for example: 01.11.02
typedef struct tagCONF_VER
{
    BYTE cfg_ver[16];
    BYTE cfg_buid[16];
} CONFIG_VER;

typedef struct tagCONFIG_SYS
{
    BYTE dev_name[32];
    WORD dev_type;
    BYTE dev_sn[16];
    BYTE rsvd[14];
} CONFIG_SYS;

typedef struct tagCONFIG_NET
{
    BYTE  host_name[32];
    BYTE  host_pos[32];
    DWORD dev_ip;
    DWORD dev_nm;	//net mask
    DWORD dev_gw;	//gate way
    BOOL  dev_dhcp;
    DWORD dhcp_id;
    DWORD dns[2];
    DWORD time_out;
    BYTE  mode;
    BYTE  dev_mac[6];
    BYTE  rsvd[13];

} CONFIG_NET;

typedef struct tagCONFIG_COM
{
    DWORD id;
    DWORD bps;
    DWORD sbit;
    BYTE  dbit;
    BYTE  chk;
    BYTE  bctrl;
    BYTE  RTS;
    BYTE  DTR;
    BYTE  all;
    BYTE  type;		//RS232/RS485
    BYTE  rsvd[5];
    DWORD alive;

} CONFIG_COM;

typedef struct tagCONFIG_SESSION
{

    BYTE  protocol;
    DWORD ip;
    DWORD port;
    BYTE  conn;
    BYTE  dis_conn;
    BYTE  time_out;

} CONFIG_SESSION;


typedef struct tagCONFIG_WKNET
{
    DWORD id;
    BYTE  mode;
    BYTE  tcp_mode;
    BYTE  CR;
    BYTE  max_num;
    BYTE  attestation;//хож╓
    BYTE  srv_1st;
    DWORD lport;
    BYTE  LF;
    BYTE  is_null;
    BYTE  reg;
    CONFIG_SESSION  session[MAX_SESSION];

} CONFIG_WKNET;


typedef struct tagCONFIG_NC
{
    DWORD id;
    CONFIG_COM com;
    CONFIG_WKNET net;
} CONFIG_NC;


typedef union tagCONF_USER_SET
{
    CONFIG_VER conf_ver;					//!< Picture capture related configure

} CONF_USER_SET, *pCONF_USET;

typedef union tagCONF_UNION
{
    CONFIG_VER conf_ver;                    //!< Configure version
    CONFIG_SYS conf_sys;
    CONFIG_NET conf_net;
//	CONFIG_NC  conf_nc[MAX_NC_PORT];
    CONFIG_COM conf_com[MAX_NC_PORT];
    CONFIG_WKNET conf_wknet[MAX_NC_PORT];
} CONF_UNION, *pCONF_UNION;

typedef struct tagCONF_DATA
{
    CONFIG_VER conf_ver;                    //!< Configure version
    CONFIG_SYS conf_sys;
    CONFIG_NET conf_net;
    CONFIG_COM conf_com[MAX_NC_PORT];
    CONFIG_WKNET conf_wknet[MAX_NC_PORT];
} CONF_DATA, *pCONF_DATA;

typedef struct tagCONF_INFO
{
    DWORD conf_size;						//!< length of configure data
    CONF_UNION conf_data;				//!< configure data
} CONF_INFO, *pCONF_INFO;

extern CONF_DATA *g_conf_info;

extern void conf_init(void);
extern void make_default_file(void);

#endif
