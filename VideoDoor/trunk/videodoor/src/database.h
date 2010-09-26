//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//



#ifndef _DATABASE_H_
#define  _DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sqlite3.h"
typedef enum tagDB_TYPE
{
    TYPE_BYTE,
    TYPE_WORD,
    TYPE_INT,
    TYPE_LLONG,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_KEY,
} DB_TYPE;


typedef struct tagDB_ARG
{
    char         arg_name[32];
    DB_TYPE  arg_type;
    int            arg_offset;
    struct tagDB_ARG * arg_nextarg;
} DB_ARG;

//strlen(path)<32
bool InitDatabase(const char *path);
//创建表
//tablename表名，db_arg表的结构
bool CreateTable(const char* tablename, const DB_ARG* db_arg);
//插入信息
//tablename表名，info信息
bool AddInfo(const char* tablename, void *info);
//查询信息
//tablename表名，pcondition\num条件信息
int QueryInfo(const char* tablename, char **pcondition, int num);
//获取信息
//tablename表名，info反馈的信息指针(外部负责)，num第num条信息
bool GetInfo(const char* tablename, void *info, int num);
//释放信息
//tablename表名
bool FreeInfo(const char* tablename);
//删除信息
//tablename表名，pcondition\num删除信息
bool DeleteInfo(const char* tablename, char **pcondition, int num);
//更新信息
//tablename表名，pcondition\num更新信息，pinfo\inum更新内容
bool UPDateInfo(const char* tablename, char **pcondition, int num, char **pinfo, int inum);

#endif
