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
//������
//tablename������db_arg��Ľṹ
bool CreateTable(const char* tablename, const DB_ARG* db_arg);
//������Ϣ
//tablename������info��Ϣ
bool AddInfo(const char* tablename, void *info);
//��ѯ��Ϣ
//tablename������pcondition\num������Ϣ
int QueryInfo(const char* tablename, char **pcondition, int num);
//��ȡ��Ϣ
//tablename������info��������Ϣָ��(�ⲿ����)��num��num����Ϣ
bool GetInfo(const char* tablename, void *info, int num);
//�ͷ���Ϣ
//tablename����
bool FreeInfo(const char* tablename);
//ɾ����Ϣ
//tablename������pcondition\numɾ����Ϣ
bool DeleteInfo(const char* tablename, char **pcondition, int num);
//������Ϣ
//tablename������pcondition\num������Ϣ��pinfo\inum��������
bool UPDateInfo(const char* tablename, char **pcondition, int num, char **pinfo, int inum);

#endif
