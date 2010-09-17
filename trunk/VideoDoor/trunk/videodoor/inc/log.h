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

typedef struct  tagLOG
{
    DHTIME mTime;
    char   mMsg[256]; 
}LOG;


#endif


