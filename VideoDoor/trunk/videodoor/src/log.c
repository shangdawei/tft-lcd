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
#include "dblog.h"


bool initLog(const char *path)
{
    return InitDBLog(path);
}

bool writeLog(LOGlv lv, char* format, ...)
{
    LOG log;
    va_list ap;

    memset(&log, 0, sizeof(LOG));

    sys_dhtime_get(&log.mTime);

    va_start ( ap, format );
    vsprintf ( log.mMsg, format, ap );
    va_end ( ap );

    return AddDBLog(&log);
}

int selectLog(DHTIME start, DHTIME end)
{
    LOG_QUERY log_query;

    memset(&log_query, 0, sizeof(LOG_QUERY));
    log_query.start_time = start;
    log_query.end_time = end;
    log_query.Istime = true;

    return QueryDBLog(&log_query);

}

bool deleteLog(DHTIME start, DHTIME end)
{
    LOG_QUERY log_query;

    memset(&log_query, 0, sizeof(LOG_QUERY));
    log_query.start_time = start;
    log_query.end_time = end;
    log_query.Istime = true;

    return DeleteDBLog(&log_query);
}

LOG *getLog(int num)
{
    static LOG log;
    memset(&log, 0, sizeof(LOG));

    if (GetDBLog(&log, num))
    {
        return &log;
    }
    else
    {
        return NULL;
    }
}

bool stopSelectLog(void)
{
    return FreeDBLog();
}


