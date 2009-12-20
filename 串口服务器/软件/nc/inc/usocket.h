#ifndef __USOCKET_H__
#define __USOCKET_H__

#include "system.h"

#define MAX_LENGTH      	1024*2			//! max packet length
//! itsip protocol version
#define ITSIP_VERSION  0x01
//! the head of itsip head length
#define ITSIP_THLEN   8

typedef enum tagITS_CMD
{
	ITS_HEAD			= 0xff,
	ITS_SYSINFO_QUERY	= 0xa1,
	ITS_VER_QUERY		= 0xa2,
	ITS_NET_QUERY		= 0xa3,
	ITS_NC_QUERY		= 0xa4,
	ITS_CONF_QUERY		= 0xa2,

	ITS_CONF_SET		= 0xa3,

	//! ack!
	ITS_ACK_SYSINFO_QUERY	= 0xb1,
	ITS_ACK_VER_QUERY		= 0xb2,
	ITS_ACK_NET_QUERY		= 0xb3,
	ITS_ACK_NC_QUERY		= 0xb4,
	ITS_ACK_CONF_QUERY		= 0xb2,

	ITS_ACK_CONF_SET		= 0xb3,
}ITS_CMD;


typedef struct tagITSIP
{
	BYTE itsip_head;     //!< head, 0xff
	BYTE itsip_cmd;      //!< command
	BYTE itsip_ver;      //!< protocol version
	BYTE itsip_thl;      //!< the head length
	WORD itsip_extlen;   //!< external data length
	BYTE itsip_dev_t;    //!< Device types
	BYTE itsip_rsvd;     //!< Reserved byte
	BYTE itsip_data[24]; //!< data
} ITSIP, *p_ITSIP;

typedef struct tagITSIP_PKT
{
	ITSIP head;				//!< ITSIP head
	BYTE data[MAX_LENGTH];	//!< external data, 1024 bytes
} ITSIP_PACKET, *p_ITSIP_PACKET;


//Net Connection information
typedef struct tagNET_CONN_INFO
{
	int conn_idx;		            //!< Index of connection
	int client_conn;	            //!< client socket descriptor
	int fd;
//	USR_INFO *user;		            //!< pointer to user data in memory
//	DOWN_STATE down_state;          //!< Video download state, 0 -- downloading, 1 -- pause, 2 -- stop
	int client_type;	            //!< client type 
//	BYTE send_mode[N_SYS_CH];		//!< file send mode, 0, waiting for search, 1 - near , 2 - far, 3 - two channel
//	BYTE file_type;		            //!< file type, 0 - picture, 1 - video, 2 -- break-point continue
//	BYTE idle;	            		//!< idle time count
	char clientip[16];
} NET_CONN_INFO, *pNET_CONN_INFO;

extern void usocket_start(void);

#endif


