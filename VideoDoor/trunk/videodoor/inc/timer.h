//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _TIMER_H
#define _TIMER_H

#include "globleinc.h"
#include "support.h"

#define SECS_PER_MIN		60
#define SECS_PER_HOUR		60 * SECS_PER_MIN
#define SECS_PER_DAY		24 * SECS_PER_HOUR

typedef struct tm              SYS_TIME;


/* system time */
typedef struct tagSYSTEM_TIME
{
    int year;
    int month;
    int day;
    int wday;
    int hour;
    int minute;
    int second;
    int isdst;
} SYSTEM_TIME;

typedef struct tagDHTIME
{
    DWORD second	: 6;		//!	second	1-60
    DWORD minute	: 6;		//!	minute	1-60
    DWORD hour		: 5;		//!	hour	1-24
    DWORD day		: 5;		//!	day 	1-31
    DWORD month		: 4;		//!	month   1-12
    DWORD year		: 6;		//!	year	2000-2063

} DHTIME, *pDHTIME;


//////////////////////////////////////////////////////////////////////////
///
///     time_printf
///     @param *time
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
void time_printf(DHTIME *time);
//////////////////////////////////////////////////////////////////////////
///
///     conf_makedefault
///     @param *dhtime, *systime
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
void sys_time_sys2dh(DHTIME *dhtime, SYSTEM_TIME *systime);
//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_diff
///     @param *start, *end
///     @return int secs
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_dhtime_diff(DHTIME *start, DHTIME *end);
//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_compare
///     @param *dhtime1, *dhtime2
///     @return COMPARE_RET
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_dhtime_compare(DHTIME *dhtime1, DHTIME *dhtime2);
//////////////////////////////////////////////////////////////////////////
///
///     sys_systime_compare
///     @param *systime1, *systime2
///     @return COMPARE_RET
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int sys_systime_compare(SYSTEM_TIME *systime1, SYSTEM_TIME *systime2);
//////////////////////////////////////////////////////////////////////////
///
///     sys_systime_get
///     @param *systime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
DWORD sys_systime_get(SYSTEM_TIME *systime);
//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime_get
///     @param *dhtime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
DWORD sys_dhtime_get(DHTIME *dhtime);
//////////////////////////////////////////////////////////////////////////
///
///     sys_dhtime2sec
///     @param *dhtime
///     @return DWORD
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
inline DWORD sys_dhtime2sec(DHTIME *dhtime);

#endif

