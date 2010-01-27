#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <pthread.h>
#include <stdio.h>

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

#define	CLOSED		1
#define	SUCCESS 	0
#define	FAILURE		(-1)

#define ON  1
#define OFF 0

typedef struct _ITS_INFO
{
    BYTE its_com_name[32]; 	        //!< 公司名称
    WORD its_dev_type;              //!< 产品型号，见表4.7:设备类型
    BYTE its_dev_sn[16];            //!< 产品唯一标识序列号
    BYTE  rsvd[14];
} ITS_INFO;

typedef struct tagTERM_REGISTER
{
    BYTE dev_mac[6];				//!< 前端设备的MAC地址
    BYTE dev_max_conn;				//!< 设备最大连接数
    BYTE reserved[9];				//!< 保留字段
    DWORD dev_ip; 					//!< 设备IP地址
    DWORD dev_sw_ver; 				//!< 设备版本号
    ITS_INFO dev_info; 				//!< 前端设备的硬件具体信息
} TERM_REGISTER;


extern DWORD sys_str2ip(BYTE *str);
extern void sys_ip2str(DWORD ip, BYTE *str);


#endif

