//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "timer.h"
#include "system.h"
//////////////////////////////////////////////////////////////////////////
///
///     time_printf
///     @param *time
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
void time_printf(DHTIME *time)
{
    fprintf(stderr, "Time: %d-%d-%d %d:%d:%d\n", time->year + 2000, time->month, time->day, time->hour, time->minute, time->second);
}

//////////////////////////////////////////////////////////////////////////
///
///     conf_makedefault
///     @param *dhtime, *systime
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
void sys_time_sys2dh(DHTIME *dhtime, SYSTEM_TIME *systime)
{
    dhtime->year   = systime->year - 2000;
    dhtime->month  = systime->month;
    dhtime->day    = systime->day;
    dhtime->hour   = systime->hour;
    dhtime->minute = systime->minute;
    dhtime->second = systime->second;
}


//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_diff
///     @param *start, *end
///     @return int secs
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_dhtime_diff(DHTIME *start, DHTIME *end)
{
    int secs = 0;

    secs = (end->day - start->day) * SECS_PER_DAY + (end->hour - start->hour) * SECS_PER_HOUR + \
           (end->minute - start->minute) * SECS_PER_MIN + (end->second - start->second);

    return secs;
}



//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_compare
///     @param *dhtime1, *dhtime2
///     @return COMPARE_RET
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_dhtime_compare(DHTIME *dhtime1, DHTIME *dhtime2)
{
    if (*((DWORD *)dhtime1) == *((DWORD *)dhtime2))
    {
        return EQUAL;
    }
    else
    {
        return *((DWORD *)dhtime1) > *((DWORD *)dhtime2) ? MORE : LESS;
    }
}

//////////////////////////////////////////////////////////////////////////
///
///     sys_systime_compare
///     @param *systime1, *systime2
///     @return COMPARE_RET
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_systime_compare(SYSTEM_TIME *systime1, SYSTEM_TIME *systime2)
{
    int ret = -1;
    DHTIME t1, t2;

    sys_time_sys2dh(&t1, systime1);
    sys_time_sys2dh(&t2, systime2);

    ret = sys_dhtime_compare(&t1, &t2);

    return ret;
}


//////////////////////////////////////////////////////////////////////////
///
///     sys_systime_get
///     @param *systime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
DWORD sys_systime_get(SYSTEM_TIME *systime)
{
    time_t timep;
    SYS_TIME *p_time = NULL;

    time(&timep);
    p_time = localtime(&timep);
    systime->year  = p_time->tm_year + 1900;
    systime->month = p_time->tm_mon + 1;
    systime->day = p_time->tm_mday;
    systime->wday = p_time->tm_wday;
    systime->hour = p_time->tm_hour;
    systime->minute = p_time->tm_min;
    systime->second = p_time->tm_sec;
    systime->isdst = p_time->tm_isdst;

    return timep;
}


//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_get
///     @param *dhtime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
DWORD sys_dhtime_get(DHTIME *dhtime)
{
    time_t timep;
    SYSTEM_TIME systime;

    timep = sys_systime_get(&systime);
    sys_time_sys2dh(dhtime, &systime);

    return timep;
}


//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime2sec
///     @param *dhtime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
inline DWORD sys_dhtime2sec(DHTIME *dhtime)
{
    DWORD secs = 0;

    secs = dhtime->hour * SECS_PER_HOUR + dhtime->minute * SECS_PER_MIN + dhtime->second;

    return secs;
}

