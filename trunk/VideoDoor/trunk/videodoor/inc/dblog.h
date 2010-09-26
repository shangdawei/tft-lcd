#ifndef _DBLOG_H_
#define _DBLOG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "log.h"


typedef struct tagLOG_QUERY
{
    DHTIME                  start_time;
    DHTIME                  end_time;
    char                       usr[32];
    bool                      Istime;//如查询时间，必须同时填写开始和结束时间
    bool                      Isusr;
} LOG_QUERY;

//初始化日志数据库，创建日志表
//path[in]保存路径
//return true 成功false失败
bool InitDBLog(const char *path);
//插入日志信息
//log[in]日志信息结构体指针
//return true 成功false失败
bool AddDBLog(LOG *log);
//查询日志信息
//query[in]查询日志结构体指针
//return -1失败，>=0符合信息的个数
int QueryDBLog(LOG_QUERY *query);
//获取日志信息
//log[out]保存的日志信息的指针，由外部负责
//num[in]查询的第num条的日志信息
//return true 成功false失败
bool GetDBLog(LOG *log, int num);
//释放日志查询的内容，和QueryDBLog成对使用
//return true 成功false失败
bool FreeDBLog(void);
//删除日志信息(deletec == NULL 删除所有日志)
//deletec[in]删除日志信息结构体指针
//return true 成功false失败
bool DeleteDBLog(LOG_QUERY *deletec);


#endif
