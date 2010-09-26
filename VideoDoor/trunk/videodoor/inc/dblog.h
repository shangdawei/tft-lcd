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
    bool                      Istime;//���ѯʱ�䣬����ͬʱ��д��ʼ�ͽ���ʱ��
    bool                      Isusr;
} LOG_QUERY;

//��ʼ����־���ݿ⣬������־��
//path[in]����·��
//return true �ɹ�falseʧ��
bool InitDBLog(const char *path);
//������־��Ϣ
//log[in]��־��Ϣ�ṹ��ָ��
//return true �ɹ�falseʧ��
bool AddDBLog(LOG *log);
//��ѯ��־��Ϣ
//query[in]��ѯ��־�ṹ��ָ��
//return -1ʧ�ܣ�>=0������Ϣ�ĸ���
int QueryDBLog(LOG_QUERY *query);
//��ȡ��־��Ϣ
//log[out]�������־��Ϣ��ָ�룬���ⲿ����
//num[in]��ѯ�ĵ�num������־��Ϣ
//return true �ɹ�falseʧ��
bool GetDBLog(LOG *log, int num);
//�ͷ���־��ѯ�����ݣ���QueryDBLog�ɶ�ʹ��
//return true �ɹ�falseʧ��
bool FreeDBLog(void);
//ɾ����־��Ϣ(deletec == NULL ɾ��������־)
//deletec[in]ɾ����־��Ϣ�ṹ��ָ��
//return true �ɹ�falseʧ��
bool DeleteDBLog(LOG_QUERY *deletec);


#endif
