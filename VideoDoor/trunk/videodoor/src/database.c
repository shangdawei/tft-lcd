//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#include "database.h"
#include "print.h"
#include "support.h"

static sqlite3 *p_sqlitedb = NULL; //数据库指针
static char *db_name = "sqlite.db";
static int power_limit = 8;

#define MAX_TABLENAMELENGTH 64
#define MAX_CONDITIONLENGTH 1024

typedef struct argQUERY_RESULT
{
    char** paszResults;
    int Rows;
    int Cols;
} QUERY_RESULT;//查询结果

typedef struct argTABLE_NAME
{
    char tablename[MAX_TABLENAMELENGTH];//表的名字
    char querycondition[MAX_CONDITIONLENGTH];//查询条件
    QUERY_RESULT result;//查询结果
    int querycount;//获取信息的次数
    int querydegree;//查询信息的次数
    int FileID;//当前的文件ID号
    const DB_ARG* db_arg;//结构体参数信息链表
    struct argTABLE_NAME* nextpname;//下一个表的指针
} TABLE_NAME;//单表结构体

static TABLE_NAME *ptable_name = NULL;
//将表的信息添加到链表中
TABLE_NAME* AddTable(const char* tablename)
{
    TABLE_NAME* phead = ptable_name;
    TABLE_NAME* ptmp = NULL;

    if (NULL == phead)
    {
        ptable_name = malloc(sizeof(TABLE_NAME));
        memset(ptable_name, 0, sizeof(TABLE_NAME));
        strncpy(ptable_name->tablename, tablename, MAX_TABLENAMELENGTH);
        ptable_name->nextpname = NULL;
        ptable_name->querydegree = - 1;
	    return ptable_name;
    }
    else
    {
        do
        {
            if (strcmp(phead->tablename, tablename) == 0)
            {
                sys_print(FUNC, WARN, "tablename = %s is created ago", tablename);
                return NULL;
            }
            else
            {
                phead = phead->nextpname;
            }
        }
        while (phead->nextpname != NULL);

        ptmp = malloc(sizeof(TABLE_NAME));
        memset(ptmp, 0, sizeof(TABLE_NAME));
        strncpy(ptmp->tablename, tablename, MAX_TABLENAMELENGTH);
        phead->nextpname = ptmp;
        ptmp->nextpname = NULL;
        ptmp->querydegree = -1;
        return ptmp;
    }
}
//获取链表中相关表的信息
TABLE_NAME* GetTable(const char* tablename)
{
    TABLE_NAME* ptmp = ptable_name;

    while (ptmp != NULL)
    {
        if (strcmp(ptmp->tablename, tablename) == 0)
        {
            return ptmp;
        }
        else
        {
            ptmp = ptmp->nextpname;
        }
    }

    sys_print(FUNC, ERROR, "no tablename = %s", tablename);
    return NULL;
}
//对数据库进行无反馈的操作
bool Operate(const char *cmd)
{
    int ret = 0;
    char *errMsg = NULL;

    if (NULL == p_sqlitedb)
    {
        sys_print(FUNC, ERROR, "p_sqlitedb == NULL");
        return false;
    }

    if (NULL == cmd)
    {
        sys_print(FUNC, ERROR, "cmd == NULL");
        return false;
    }

    ret = sqlite3_exec(p_sqlitedb, cmd, NULL, NULL, &errMsg);

    if (ret != SQLITE_OK)
    {
        sys_print(FUNC, ERROR, errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
//查询个数回调函数
int sqlitecount_callback(void *pArg, int argc, char **argv, char **columnNames)
{
    int *num;
    num = (int *)pArg;
    *num = (int)atoi(argv[0]);
    return 0;
}
//对数据库进行查询长度的操作
int Query_Count(const char *cmd)
{
    int ret = 0;
    int count = 0;
    char *errMsg = NULL;

    if (NULL == p_sqlitedb)
    {
        sys_print(FUNC, ERROR, "p_sqlitedb == NULL");
        return -1;
    }

    if (NULL == cmd)
    {
        sys_print(FUNC, ERROR, "cmd == NULL");
        return -1;
    }

    ret = sqlite3_exec(p_sqlitedb, cmd, sqlitecount_callback, &count, &errMsg);

    if (ret != SQLITE_OK)
    {
        sys_print(FUNC, ERROR, errMsg);
        sqlite3_free(errMsg);
        return -1;
    }

    return count;
}
//对数据库进行查询信息的操作
bool Query_Result(const char *cmd, QUERY_RESULT* result)
{
    int ret = 0;
    char *errMsg = NULL;

    if (NULL == p_sqlitedb)
    {
        sys_print(FUNC, ERROR, "p_sqlitedb == NULL");
        return false;
    }

    if (NULL == result)
    {
        sys_print(FUNC, ERROR, "result == NULL");
        return false;
    }

    if (result->paszResults != NULL)
    {
        sqlite3_free_table(result->paszResults);
        result->paszResults = NULL;
    }

    result->Cols = 0;
    result->Rows = 0;
    sys_print(FUNC, DBG, "%s", (char *)cmd);
    ret = sqlite3_get_table(p_sqlitedb, cmd, &(result->paszResults), &(result->Rows), &(result->Cols), &errMsg);

    if (ret != SQLITE_OK)
    {
        sys_print(FUNC, ERROR, "%s", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
//获取查询结果
bool Get_Result(TABLE_NAME *tablename, void *info, int num)
{
    const DB_ARG* ptmp = tablename->db_arg;
    unsigned char *pdata = (unsigned char *)info;
    int col = 0;

    if (NULL == tablename || NULL == info || num < 0 || tablename->result.Rows < 1 || num > tablename->result.Rows)
    {
        sys_print(FUNC, ERROR, "tablename = %d info= %d  num = %d result->Rows = %d\n", tablename, info, num, tablename->result.Rows);
        sys_print(FUNC, ERROR, "condition is error ");
        return false;
    }

    while (ptmp != NULL)
    {
        switch (ptmp->arg_type)
        {
        case TYPE_BYTE:
            *(unsigned char *)(pdata + (ptmp->arg_offset)) = (unsigned char)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_WORD:
            *(unsigned short *)(pdata + (ptmp->arg_offset)) = (unsigned short)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_INT:
            *(unsigned int *)(pdata + (ptmp->arg_offset)) = (unsigned int)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_LLONG:
            *(unsigned long long *)(pdata + (ptmp->arg_offset)) = (unsigned long long)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_DOUBLE:
            *(double *)(pdata + (ptmp->arg_offset)) = (double)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_STRING:
            strcpy((char *)(pdata + (ptmp->arg_offset)), tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        case TYPE_KEY:
            tablename->FileID = (unsigned int)atoi(tablename->result.paszResults[num*(tablename->result.Cols)+col]);
            break;
        default:
            sys_print(FUNC, ERROR, "arg no this type");
            break;
        }

        col++;
        ptmp = ptmp->arg_nextarg;
    }

    return true;
}
//释放查询结果
bool  FreeTableResult(TABLE_NAME *table)
{
    if (NULL == table)
    {
        return false;
    }

    memset(table->querycondition, 0, MAX_CONDITIONLENGTH);
    table->querydegree = -1;
    table->querycount = 0;
    table->FileID = 0;

    if (table->result.paszResults != NULL)
    {
        sqlite3_free_table(table->result.paszResults);
        table->result.paszResults = NULL;
    }

    table->result.Cols = 0;
    table->result.Rows = 0;
    return true;
}


//public
bool InitDatabase(const char *path)
{
    int ret = 0;
    char dbname[64];
    memset(dbname, 0, sizeof(db_name));

    if (NULL == p_sqlitedb)
    {
        strcat(dbname, path);
        strcat(dbname, db_name);
        ret = sqlite3_open(dbname, &p_sqlitedb);

        if (ret != SQLITE_OK)
        {
            sys_print(FUNC, ERROR, "database open error");
            return false;
        }
    }

    return true;
}
bool CreateTable(const char* tablename, const DB_ARG* db_arg)
{
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    const DB_ARG* ptmp = NULL;
    TABLE_NAME* ptable = AddTable(tablename);

    if (NULL == ptable)
    {
        return false;
    }

    ptable->db_arg = db_arg;
    ptmp = ptable->db_arg;
    strcat(sqlcmd, "CREATE TABLE ");
    strcat(sqlcmd, tablename);
    strcat(sqlcmd, " ( ");

    while (ptmp != NULL)
    {
        strcat(sqlcmd, ptmp->arg_name);

        switch (ptmp->arg_type)
        {
        case TYPE_BYTE:
            strcat(sqlcmd, " VARCHAR ");
            break;
        case TYPE_WORD:
        case TYPE_INT:
        case TYPE_LLONG:
            strcat(sqlcmd, " INTEGER ");
            break;
        case TYPE_DOUBLE:
            strcat(sqlcmd, " DOUBLE ");
            break;
        case TYPE_STRING:
            strcat(sqlcmd, " STRING ");
            break;
        case TYPE_KEY:
            strcat(sqlcmd, " INTEGER PRIMARY KEY AUTOINCREMENT ");
            break;
        default:
            sys_print(FUNC, ERROR, "arg no this type");
            break;
        }

        if (ptmp->arg_nextarg != NULL)
        {
            strcat(sqlcmd, ",");
        }

        ptmp = ptmp->arg_nextarg;
    }

    strcat(sqlcmd, " ) ");
    return Operate(sqlcmd);
}
bool AddInfo(const char* tablename, void *info)
{
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    char tmp[128];
    memset(tmp, 0, sizeof(tmp));
    unsigned char *pdata = (unsigned char *)info;
    const DB_ARG* ptmp = NULL;
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename)
    {
        return false;
    }

    ptmp = ptablename->db_arg;
    strcat(sqlcmd, "INSERT INTO ");
    strcat(sqlcmd, tablename);
    strcat(sqlcmd, " VALUES ( ");

    while (ptmp != NULL)
    {
        switch (ptmp->arg_type)
        {
        case TYPE_BYTE:
            sprintf(tmp, "%d", *(unsigned char *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_WORD:
            sprintf(tmp, "%d", *(unsigned short *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_INT:
            sprintf(tmp, "%d", *(unsigned int *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_LLONG:
            sprintf(tmp, "%llu", *(unsigned long long *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_DOUBLE:
            sprintf(tmp, "%f", *(double *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_STRING:
            sprintf(tmp, "'%s'", (char *)(pdata + (ptmp->arg_offset)));
            break;
        case TYPE_KEY:
            sprintf(tmp, " NULL ");
            break;
        default:
            sys_print(FUNC, ERROR, "arg no this type");
            break;
        }

        strcat(sqlcmd, tmp);

        if (ptmp->arg_nextarg != NULL)
        {
            strcat(sqlcmd, ",");
        }

        ptmp = ptmp->arg_nextarg;
    }

    strcat(sqlcmd, " ) ");
    return Operate(sqlcmd);
}

int QueryInfo(const char* tablename, char **pcondition, int num)
{
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    int i = 0;
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename)
    {
        return -1;
    }

    FreeTableResult(ptablename);
    strcat(sqlcmd, "SELECT COUNT(*) FROM ");
    strcat(sqlcmd, tablename);

    if (pcondition != NULL && num > 0)
    {
        strcat(ptablename->querycondition, " WHERE ");

        for (i = 0; i < num; i++)
        {
            strcat(ptablename->querycondition, pcondition[i]);

            if (i != (num - 1))
            {
                strcat(ptablename->querycondition, " AND ");
            }
        }
    }

    strcat(sqlcmd, ptablename->querycondition);
    ptablename->querycount = Query_Count(sqlcmd);
    return ptablename->querycount;

}
bool GetInfo(const char* tablename, void *info, int num)
{
    bool ret = true;
    int length = 0;
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    char tmp[32];
    memset(tmp, 0, sizeof(tmp));
    const DB_ARG* ptmp = NULL;
    int num_tmp = num;
    static int offset = 0;
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename || ptablename->querycount <= 0 || num > ptablename->querycount)
    {
        return false;
    }

    if ((num_tmp >> power_limit) != ptablename->querydegree)
    {
        ptmp = ptablename->db_arg;

        while (ptmp != NULL)
        {
            if (ptmp->arg_type == TYPE_KEY)
            {
                offset = ptmp->arg_offset;
                break;
            }
            else
            {
                ptmp = ptmp->arg_nextarg;
            }
        }

        ptablename->querydegree = (num_tmp >> power_limit);
        memset(sqlcmd, 0, sizeof(sqlcmd));
        memset(tmp, 0, sizeof(tmp));
        strcat(sqlcmd, " SELECT * FROM ");
        strcat(sqlcmd, tablename);
        strcat(sqlcmd, ptablename->querycondition);
        sprintf(tmp, " AND %s > %d ", ptmp->arg_name, ptablename->FileID);
        strcat(sqlcmd, tmp);
        sprintf(tmp, " LIMIT 0, %d", 1 << power_limit);
        strcat(sqlcmd, tmp);
        Query_Result(sqlcmd, &ptablename->result);
    }

    length = num - ((ptablename->querydegree) << power_limit);
    ret = Get_Result(ptablename, info, length);
    return ret;
}

bool FreeInfo(const char* tablename)
{
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename)
    {
        return false;
    }

    return FreeTableResult(ptablename);

}

bool DeleteInfo(const char* tablename, char **pcondition, int num)
{
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    int i = 0;
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename)
    {
        return false;
    }

    strcat(sqlcmd, "DELETE FROM ");
    strcat(sqlcmd, tablename);

    if (pcondition != NULL && num > 0)
    {
        strcat(sqlcmd, " WHERE ");

        for (i = 0; i < num; i++)
        {
            strcat(sqlcmd, pcondition[i]);

            if (i != (num - 1))
            {
                strcat(sqlcmd, " AND ");
            }
        }
    }

    return Operate(sqlcmd);
}

bool UPDateInfo(const char* tablename, char **pcondition, int num, char **pinfo, int inum)
{
    char sqlcmd[1024];
    memset(sqlcmd, 0, sizeof(sqlcmd));
    int i = 0;
    TABLE_NAME* ptablename = GetTable(tablename);

    if (NULL == ptablename)
    {
        return false;
    }

    strcat(sqlcmd, "UPDATE ");
    strcat(sqlcmd, tablename);

    if (pinfo != NULL && inum > 0)
    {
        strcat(sqlcmd, " SET ");

        for (i = 0; i < inum; i++)
        {
            strcat(sqlcmd, pcondition[i]);

            if (i != (inum - 1))
            {
                strcat(sqlcmd, " , ");
            }
        }
    }

    if (pcondition != NULL && num > 0)
    {
        strcat(sqlcmd, " WHERE ");

        for (i = 0; i < num; i++)
        {
            strcat(sqlcmd, pcondition[i]);

            if (i != (num - 1))
            {
                strcat(sqlcmd, " AND ");
            }
        }
    }

    return Operate(sqlcmd);
}



