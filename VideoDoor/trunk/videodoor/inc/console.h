//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "globleinc.h"
#include "support.h"

#define	 RXBUF_SZ		128
#define CON_LISTEN_PORT	6000
#define NAME_LEN		32

enum {
	CON_OFF = 0,
	CON_USRNAME,
	CON_PASSWD,
	CON_MENU,
	CON_LINE,
};

enum {
  ASCII_CR	=	0x0a,
  ASCII_LF	=	0x0d,
  ASCII_BS	=	0x08,
  ASCII_BELL=	0x07,
  ASCII_TAB	=	0x09,
  ASCII_XON	=	0x11,
  ASCII_XOFF=	0x13,
  ASCII_ESC	=	0x1B,
  ASCII_DEL	=	0x7F,
  ASCII_BACK=	0x08,
};

enum {
 LVL_SYS	=	0,
 LVL_USR	=	3,
};
typedef struct tagCONSOLE
{
	char	name[32];				//!< 登陆用户名称	
	BYTE	uid;					//!< 登陆者权限
	BYTE	right;					//!< 登陆者权限
	BYTE	status;					//!< 控制台状态
	BYTE	rxlen;					//!< 接收总长
	char	rxbuf[RXBUF_SZ];		//!< 接收缓冲
	BYTE	params;					//!< 参数数量
	char	partab[8][32];			//!< 参数列表
	BYTE    idletime;
} CONSOLE;

#define USR_NAME_LEN 10
#define USR_PSW_LEN  15

typedef struct tagUSR_INFO
{
	WORD usr_id;
// 	BYTE usr_id;                   //!< user ID
//	BYTE usr_grp_id;	           //!< user group, such as "admin"
	BYTE usr_name[USR_NAME_LEN];   //!< user name
	BYTE usr_passwd[USR_PSW_LEN];  //!< password
	BYTE usr_auth;	               //!< user authority, 0 - guest, 1 - normal user，2-administrator，3-super user
	BYTE usr_state;                //!< user state
	BYTE reserved[3];	           //!< 
} USR_INFO, *p_USR_INFO;

enum USR_STATUS
{
	S_FREE          = 0x00,   //!< free user, not login
	S_PSW_ERR       = 0x01,   //!< password error
	S_NO_USR        = 0x02,   //!< no such user in system
	S_ACTIVE        = 0x03,   //!< active user, has logined
	S_USR_LOCKED    = 0x04,   //!< user has been locked
	S_IN_BLACKLIST  = 0x05,   //!< user in blacklist
	S_USR_EXISTED   = 0x06,	  //!< user has existed
	S_NO_AUTH       = 0x07,   //!< authority low
	S_USR_OVER      = 0x08,   //!< User count reach to the limit
	S_GUEST         = 0x0a,   //!< guest user, have limit authority
	S_USER          = 0x14,   //!< normal user
	S_ADMIN         = 0x50,   //!< system administrator
	S_SUPER         = 0x63,   //!< super user, have the unlimit authority
};
typedef struct {
	int		level;					//!< 等级
	char	*ParseStr;
	void	(*ParseProc)();
	char	*HelpMsg;
} PAR_TAB;


//////////////////////////////
void console_init(void);


#endif

