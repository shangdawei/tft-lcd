//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _LOG_H
#define _LOG_H

#include "globleinc.h"
#include "support.h"
#include "timer.h"

#define  LOG_FILE   "videodoor.log"

typedef enum  tagLOGlv
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
}LOGlv;

typedef struct  tagLOG
{
    DHTIME mTime;
    LOGlv  lv;    
    char   user[32];
    char   mMsg[256];
} LOG;

bool initLog(const char *path);
bool writeLog(LOGlv lv, char* format, ...);
int selectLog(DHTIME start, DHTIME end);
bool deleteLog(DHTIME start, DHTIME end);
LOG *getLog(int num);
bool stopSelectLog(void);

#endif


