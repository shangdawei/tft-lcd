//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef	_PRINT_H_
#define	_PRINT_H_

#include "globleinc.h"


int logTrace(const char* fmt, ...);
int logDebug(const char* fmt, ...);
int logInfo(const char* fmt, ...);
int logWarn(const char* fmt, ...);
int logError(const char* fmt, ...);
int logFatal(const char* fmt, ...);


#ifdef __linux__ // perform complete xxxxf macro replacement
#ifdef DEBUG
#	define tracef(format, ...) logTrace(format, ## __VA_ARGS__)
#	define debugf(format, ...) logDebug(format, ## __VA_ARGS__)
#	define infof(format, ...) logInfo(format, ## __VA_ARGS__)
#	define warnf(format, ...) logWarn(format, ## __VA_ARGS__)
#	define errorf(format, ...) logError(format, ## __VA_ARGS__)
#	define fatalf(format, ...) logFatal(format, ## __VA_ARGS__)
#else
#	define tracef(format, ...) 
#	define debugf(format, ...) 
#	define infof(format, ...) logInfo(format, ## __VA_ARGS__)
#	define warnf(format, ...) logWarn(format, ## __VA_ARGS__)
#	define errorf(format, ...) logError(format, ## __VA_ARGS__)
#	define fatalf(format, ...) logFatal(format, ## __VA_ARGS__)
#endif
#else
#ifdef DEBUG
#	define tracef logTrace
#	define debugf logDebug
#	define infof logInfo
#	define warnf logWarn
#	define errorf logError
#	define fatalf logFatal
#else
#	define tracef if(0)logTrace
#	define debugf if(0)logDebug
#	define infof logInfo
#	define warnf logWarn
#	define errorf logError
#	define fatalf logFatal
#endif
#endif

//#define tracepoint()  debugf("tracepoint: %s, %d.\n",__FILE__,__LINE__)
#define tracepoint()  sys_print(FUNC, DBG, "tracepoint: %s, %d.\n",__FILE__,__LINE__)

typedef enum tagLOG_Lv
{
    LNULL,    
    TRACE,
    DBG,
    INFO,
    WARN,
    ERROR,
    FATAL,

}LOG_Lv;

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
int sys_print ( char* module, LOG_Lv level, char* format, ... );


#endif


