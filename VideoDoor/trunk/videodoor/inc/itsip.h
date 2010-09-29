//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _ITSIP_H
#define _ITSIP_H

#include "globleinc.h"
#include "support.h"


enum ITS_CMD
{
    ITS_HEAD              = 0xff,//!< ITSIP head
    ITS_DOOR_ASK          = 0xa0,//!< ·Ã¿Í·ÃÎÊ
    ITS_DOOR_OPEN         = 0xa1,//!< System Information Query
    ITS_CONF_QUERY        = 0xa2,//!< Configuration Query
    ITS_CONF_SET          = 0xa3,//!< Configuration Setup
    ITS_PIC_QUERY         = 0xa4,//!< Picture Query
    ITS_VIDEO_QUERY       = 0xa5,//!< Video Query
    ITS_LOG_QUERY         = 0xa6,//!< System Log Query
    ITS_UPLOAD_REQ        = 0xa7,//!< Data Upload
    ITS_TEMP_UPLOAD       = 0xa8,//!< Temperature real time upload
    ITS_USER_SET          = 0xa9,//!< User account setup
    ITS_SYS_UP            = 0xaa,//!< System upgrade
    ITS_SYSUP_LOGIN       = 0xab,//!< System upgrade login
    ITS_MAN_CAP           = 0xac,//!< Manual Capture
    ITS_CONN_TEST         = 0xaf,//!< Network connection test
    ITS_DOWNLOAD          = 0xd1,//!< File download
    ITS_DELETE            = 0xde,//!< File remove
    ITS_LOGOUT            = 0xe0,//!< Logout
    ITS_TIME_SET          = 0xc2,//!< Time setup
    ITS_TALK_REQ          = 0xc3,//!< Talking request
    ITS_AUDIO             = 0xc4,//!< Audio translation
    ITS_DEV_CTL           = 0xc5,//!< Devices control
    ITS_DEV_CHECK         = 0xc6,//!< Devices Check
    ITS_REGISTER          = 0xc7,//!< Auto register
    ITS_KEEPLIVE          = 0xc8,//!< Keep alive
    ITS_SPEED_REQ         = 0xc9,//!< Speed request
    ITS_ALMDEV_SET        = 0xca,//!< Alarm devices set
    ITS_ALM_IN            = 0xcb,//!< Alarm input
    ITS_ALM_OUT           = 0xcc,//!< Alarm output
    ITS_WORK_QUERY        = 0xcd,//!< System status query
    ITS_SM_QUERY          = 0x81,//!< speed measure parameter query
    ITS_PIC_UPLOAD        = 0xe1,//!< Picture real-time upload request
    ITS_VID_UPLOAD        = 0xe2,//!< Video real-time upload request
    ITS_VID_PLAY          = 0xe3,//!< Video records play via network request
    ITS_DOUBLE_SID		  = 0xe4,//!< System serial numbers query
    ITS_TI_TEST			  = 0xfe,//!< Test Command for dm355 plat

    ITS_CPS_SNAP          = 0xe9,

    //tmy add by 2010-04-08
    ITS_LOG_UDP            = 0xea,//! upload log by udp


    /*! \note command definitions of ACK
    */
    ITS_ACK_DOOR_ASK      = 0xb0,/*!< ACK for user Login */
    ITS_ACK_DOOR_OPEN     = 0xb1,/*!< ACK for System information query */
    ITS_ACK_CONF_QUERY    = 0xb2,/*!< ACK for configuration query */
    ITS_ACK_CONF_SET      = 0xb3,/*!< ACK for configuration setup */
    ITS_ACK_PIC_QUERY     = 0xb4,/*!< ACK for picture query */
    ITS_ACK_VIDEO_QUERY   = 0xb5,/*!< ACK for video query */
    ITS_ACK_LOG_QUERY     = 0xb6,/*!< ACK for log query */
    ITS_ACK_UPLOAD_REQ    = 0xb7,/*!< ACK for auto upload request */
    ITS_ACK_TEMP_UPLOAD   = 0xb8,/*!< ACK for temperature upload */
    ITS_ACK_USER_SET      = 0xb9,/*!< ACK for user account setup */
    ITS_ACK_SYS_UP        = 0xba,/*!< ACK for system upgrade */
    ITS_ACK_SYSUP_LOGIN   = 0xbb,/*!< ACK for system upgrade login */
    ITS_ACK_MAN_CAP       = 0xbc,/*!< ACK for manual capture */
    ITS_ACK_DOWNLOAD      = 0xbd,/*!< ACK for file download */
    ITS_ACK_DELETE        = 0xbe,/*!< ACK for file remove */
    ITS_ACK_TIME_SET      = 0xd2,/*!< ACK for time setup */
    ITS_ACK_TALK_REQ      = 0xd3,/*!< ACK for talking request */
    ITS_ACK_AUDIO         = 0xd4,/*!< ACK for audio */
    ITS_ACK_DEV_CTL       = 0xd5,/*!< ACK for device control */
    ITS_ACK_DEV_CHECK     = 0xd6,/*!< ACK for device check */
    ITS_ACK_REGISTER      = 0xd7,/*!< ACK for its devices register */
    ITS_ACK_KEEPLIVE      = 0xd8,/*!< ACK for keep-alive */
    ITS_ACK_SPEED_REQ     = 0xd9,/*!< ACK for speed request */
    ITS_ACK_ALMDEV_SET    = 0xda,/*!< ACK for alarm device setup */
    ITS_ACK_ALM_IN        = 0xdb,/*!< ACK for alarm input */
    ITS_ACK_ALM_OUT       = 0xdc,/*!< ACK for alarm output */
    ITS_ACK_WORK_QUERY    = 0xdd,/*!< ACK for devices work states */
    ITS_ACK_LOGOUT        = 0xde,/*!< ACk for logout */
    ITS_ACK_SM_QUERY      = 0xfd,/*!< ACK for speed measure information query */
    ITS_ACK_PIC_UPLOAD    = 0xf1,/*!< ACK for Picture real-time upload request */
    ITS_ACK_VID_UPLOAD    = 0xf2,/*!< ACK for Video real-time upload request */
    ITS_ACK_VID_PLAY      = 0xf3,/*!< ACK for Video records play via network request */
    ITS_ACK_DOUBLE_SID	  = 0xf4,/*!< ACK for System serial numbers query*/


    //tmy add by 2010-04-08
    ITS_ACK_LOG_UDP            = 0xfa,/*!<ACK for upload log by udp>*/
};

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

#endif

