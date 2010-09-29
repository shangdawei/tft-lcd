//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#include "dblog.h"
#include "database.h"
#include <linux/stddef.h>

static const char *log_table = "Log";
#define MAX_CODLENFTH 64
DB_ARG *InitArg(void)
{
    DB_ARG *tmp = NULL;
    DB_ARG *phead = NULL;
    DB_ARG *plog_arg = NULL;

    tmp = malloc(sizeof(DB_ARG));
    memset(tmp, 0, sizeof(DB_ARG));
    strncpy(tmp->arg_name, "FileID", 32);
    tmp->arg_type = TYPE_KEY;
    tmp->arg_offset = 0;
    plog_arg = tmp;
    phead = tmp;

    tmp = malloc(sizeof(DB_ARG));
    memset(tmp, 0, sizeof(DB_ARG));
    strncpy(tmp->arg_name, "mTime", 32);
    tmp->arg_type = TYPE_INT;
    tmp->arg_offset = offsetof(LOG, mTime);
    phead->arg_nextarg = tmp;
    phead = tmp;

    tmp = malloc(sizeof(DB_ARG));
    memset(tmp, 0, sizeof(DB_ARG));
    strncpy(tmp->arg_name, "user", 32);
    tmp->arg_type = TYPE_STRING;
    tmp->arg_offset = offsetof(LOG, user);
    phead->arg_nextarg = tmp;
    phead = tmp;

    tmp = malloc(sizeof(DB_ARG));
    memset(tmp, 0, sizeof(DB_ARG));
    strncpy(tmp->arg_name, "mMsg", 32);
    tmp->arg_type = TYPE_STRING;
    tmp->arg_offset = offsetof(LOG, mMsg);
    phead->arg_nextarg = tmp;
    phead = tmp;
    phead->arg_nextarg = NULL;

    return plog_arg;

}

bool InitDBLog(const char *path)
{
    if (NULL == path)
    {
        return false;
    }

    if (!InitDatabase(path))
    {
        return false;
    }

    DB_ARG *plog_arg = InitArg();

    if (NULL == plog_arg)
    {
        return false;
    }

    return CreateTable(log_table, plog_arg);
}
bool AddDBLog(LOG *log)
{
    if (NULL == log)
    {
        return false;
    }

    return AddInfo(log_table, (void *)log);
}
int QueryDBLog(LOG_QUERY *query)
{
    int num = 0;
    int i = 0;
    char **pcondition = NULL;

    if (NULL == query)
    {
        return -1;
    }

    if (query->Istime == true)
    {
        num++;
    }

    if (query->Isusr == true)
    {
        num++;
    }

    if (num > 0)
    {
        pcondition = malloc(sizeof(char *) * num);
        memset(pcondition, 0, sizeof(char *)*num);

        for (i = 0; i < num; i++)
        {
            pcondition[i] = malloc(MAX_CODLENFTH);
            memset(pcondition[i], 0, MAX_CODLENFTH);
        }

        i = 0;

        if (query->Istime == true)
        {
            snprintf(pcondition[i], MAX_CODLENFTH, "mTime > %d AND mTime < %d", *(DWORD*)&(query->start_time), *(DWORD*)&(query->end_time));
            i++;
        }

        if (query->Isusr == true)
        {
            snprintf(pcondition[i], MAX_CODLENFTH, "user = '%s' ", query->usr);
            i++;
        }
    }

    return QueryInfo(log_table, pcondition, num);
}

bool GetDBLog(LOG *log, int num)
{
    return GetInfo(log_table, (void *)log, num);
}

bool FreeDBLog(void)
{
    return FreeInfo(log_table);
}

bool DeleteDBLog(LOG_QUERY *deletec)
{
    if (NULL == deletec)
    {
        return DeleteInfo(log_table, NULL, 0);;
    }

    int num = 0;
    int i = 0;
    char **pcondition = NULL;

    if (deletec->Istime == true)
    {
        num++;
    }

    if (deletec->Isusr == true)
    {
        num++;
    }

    if (num > 0)
    {
        pcondition = malloc(sizeof(char *) * num);
        memset(pcondition, 0, sizeof(char *)*num);

        for (i = 0; i < num; i++)
        {
            pcondition[i] = malloc(MAX_CODLENFTH);
            memset(pcondition[i], 0, MAX_CODLENFTH);
        }

        i = 0;

        if (deletec->Istime == true)
        {
            snprintf(pcondition[i], MAX_CODLENFTH, "mTime > =%d AND mTime =< %d", *(DWORD*)&(deletec->start_time), *(DWORD*)&(deletec->end_time));
            i++;
        }

        if (deletec->Isusr == true)
        {
            snprintf(pcondition[i], MAX_CODLENFTH, "user = '%s' ", deletec->usr);
            i++;
        }
    }

    return DeleteInfo(log_table, pcondition, num);
}


