//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "print.h"
#include "support.h"

// 记录上次输出最后一个字符是否是换行，还不完全可靠
static BOOL returned = TRUE;

#ifdef WIN32
#define LOG_MSG(x, y) \
	char buffer[8192]; \
	if(returned) \
	{ \
        struct timeval tv;\
		struct tm t;\
		gettimeofday(&tv,NULL);\
		t = *(localtime(&tv.tv_sec));\
		printf("%02d:%02d:%02d|", t.tm_hour, t.tm_min, t.tm_sec); \
		printf(x); \
	} \
	size_t n; \
	va_list ap; \
	va_start(ap, fmt); \
	n = vsprintf(buffer, fmt, ap); \
	assert(n <= 8192); \
	fputs(buffer, stdout); \
	returned = (n && buffer[n - 1] == '\n'); \
	return(n);
#else //#ifdef WIN32
#define LOG_MSG(x, y) \
	char buffer[8192]; \
	if(returned) \
	{ \
		printf("\033[%d;40m", y);\
		struct timeval tv;\
		struct tm t;\
		gettimeofday(&tv,NULL);\
		t = *(localtime(&tv.tv_sec));\
		printf("%02d:%02d:%02d|", t.tm_hour, t.tm_min, t.tm_sec); \
		printf(x); \
	} \
	size_t n; \
	va_list ap; \
	va_start(ap, fmt); \
	n = vsnprintf(buffer, 8192, fmt, ap); \
	assert(n <= 8192); \
	fputs(buffer, stdout); \
	returned = (n && buffer[n - 1] == '\n'); \
	if(returned)printf("\033[0m");\
	return(n);
#endif //#ifdef WIN32


int logTrace(const char* fmt, ...)
{
    LOG_MSG("trace ", 37);
}

int logDebug(const char* fmt, ...)
{
    LOG_MSG("debug ", 36);
}

int logInfo(const char* fmt, ...)
{
    LOG_MSG("info  ", 32);
}

int logWarn(const char* fmt, ...)
{
    LOG_MSG("warn  ", 33);
}

int logError(const char* fmt, ...)
{
    LOG_MSG("error ", 31);
}

int logFatal(const char* fmt, ...)
{
    LOG_MSG("fatal ", 35);
}



//////////////////////////////////////////////////////////////////////////
///
///     打印信息
///     @param  module  函数名称
///     @param  level   日志等级
///     @param  format  数据内容
///     @return SUCCESS FAILURE
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-16
//////////////////////////////////////////////////////////////////////////
int sys_print ( char* module, LOG_Lv level, char* format, ... )
{
    char szbuf[1024*8+256] = "", szcontent[1024*8] = "";
    va_list ap;
    struct timeval tv;
    struct tm stm;
    const char * g_log_lv[] =
    {
        "NULL",
        "TRACE",
        "DBG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
    };

    const int color[] = {0, 37, 36, 32, 33, 31, 35};
    
    va_start ( ap, format );
    vsprintf ( szcontent, format, ap );
    va_end ( ap );

    gettimeofday ( &tv, NULL );
    stm = * ( localtime ( &tv.tv_sec ) );

    if(szcontent[strlen(szcontent) - 1] == '\n') 
        szcontent[strlen(szcontent) - 1] = '\0';
    
    sprintf ( szbuf, "\033[%d;40m%04d-%02d-%02d %02d:%02d:%02d:%06ld---[%s][%s] %s\033[0m\n",color[level],
                stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec, tv.tv_usec,
                module, g_log_lv[level], szcontent );
        
    printf ( "%s", szbuf );
    return 1;
}

