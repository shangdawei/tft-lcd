#ifndef	__LUCLI_H__
#define	__LUCLI_H__

#define MAX_LENGTH          (1024*2)            //! max packet length
//! itsip protocol version
#define ITSIP_VERSION  0x01
////! the head of itsip head length
#define ITSIP_THLEN   8
#define ITS_HEAD    0xFF
#define ITS_VER_QUERY   0xa2
#define ITS_VER_SET		0xa3

#define	UCLI_PATH	"/tmp/foo.socket"

#define	MAX_NC_PORT	16
#define	MAX_SESSION	6

//! type redefinition 

typedef unsigned char           BYTE;
typedef unsigned short          WORD;
typedef signed int              LONG;
typedef unsigned int            DWORD;
typedef unsigned int            PARAM;
typedef void *                  HANDLE;
typedef BYTE *                  STRING;
typedef signed int              BOOL;
typedef unsigned long long      UQWORD;
typedef long long               SQWORD;

typedef DWORD                   COLORREF;
typedef DWORD                   COLORDEV;

enum CONF_LIST 
{
	CONF_ALL         = 0x00,                //!< All configure
	CONF_VER         = 0x01,                //!< Picture capture configure
	CONF_SYS		 = 0x02,
	CONF_NET         = 0x03,                //!< Video Record configure
	CONF_NC      	 = 0x04,                //!< Speed measure configure
	CONF_COM		 = 0x05,
	CONF_WKNET		 = 0x06,
};							                           //!< Configure types count
													   //}; /* CONF_LIST end */
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
	float sbit;
	BYTE  dbit;
	BYTE  chk;
	BYTE  bctrl;
	BYTE  RTS;
	BYTE  DTR;
	BYTE  all;
	BYTE  type;		//RS232/RS485
	BYTE  rsvd[5];
	DWORD alive;

}CONFIG_COM;

typedef struct tagCONFIG_SESSION
{
	
	BYTE  protocol;
	DWORD ip; 
	DWORD port;
	BYTE  conn;
	BYTE  dis_conn;
	BYTE  time_out;

}CONFIG_SESSION;


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

}CONFIG_WKNET;

typedef struct tagCONFIG_NC
{
	DWORD id;
	CONFIG_COM com;
	CONFIG_WKNET net;
}CONFIG_NC;

extern int cli_conn(const char *name);
extern int get_net(int fd, CONFIG_NET *pnet);
extern int get_com(int fd, CONFIG_COM *pcom);
extern int set_com(int fd, CONFIG_COM *pcom);
extern int get_wknet(int fd, CONFIG_WKNET *pcom);
extern int get_wknet(int fd, CONFIG_WKNET *pcom);

extern int set_net(int fd, CONFIG_NET *pnet);
extern void sys_ip2str(DWORD ip, BYTE *str);
extern DWORD sys_str2ip(BYTE *str);
#endif

