//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "log.h"
#include "timer.h"


int writeLog(char* format, ...)
{
    LOG log;
    va_list ap;
    
    sys_dhtime_get(&log.mTime);

       
    va_start ( ap, format );
    vsprintf ( log.mMsg, format, ap );
    va_end ( ap );
}

