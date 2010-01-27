/*
**
** thread.c
**
*/
#include "thread.h"




void trdinfo_on(TRD_LST* ptrdlst)
{
    ptrdlst->id = pthread_self();
    ptrdlst->status = ON;
    strcpy(ptrdlst->msg, "thread is running");
}


void trdinfo_off(TRD_LST* ptrdlst, char *msg)
{
    ptrdlst->status = OFF;
    strcpy(ptrdlst->msg, msg);
}



/*	$Function	:	trd_create
==	====================================================================================
==	Description	:	create thread
==	Argument	:	trd_id	:	[in]	thread id
				:	func	:	[in]	thread routine
==	Return		:	0, success; -1, thread create error; -2, thread detach error
==	Modification:	2007/04/20	xuw	create
==	Note		:
==	====================================================================================
*/
int trd_create(TRD_t *trd_id, void *(*func)(void *))
{
    int ret = -1;

    ret = pthread_create(trd_id, NULL, func, NULL);

    if (ret != 0)
    {
        perror("trd_create: pthread_create");
        return -1;
    }

    ret = pthread_detach(*trd_id);

    if (ret != 0)
    {
        perror("trd_create: pthread_detach");
        return -2;
    }

    return 0;
}

/* ==============================================================================================*/
/**
* @brief	:	create thread
*
* @param	:	pLst
* @param	:	func
* @param	:	fun_name
*
* @return	:	0, success; -1, thread create error; -2, thread detach error
*/
/* ==============================================================================================*/
int trd_create2(TRD_LST * pLst, void *(*func)(void *), void* arg, char * fun_name)
{
    int ret = -1;

    ret = pthread_create(&pLst->trd_id, NULL, func, arg);

    if (ret != 0)
    {
        pLst->status = OFF;
        perror("trd_create: pthread_create");
        return -1;
    }

    ret = pthread_detach(pLst->trd_id);

    if (ret != 0)
    {
        pLst->status = OFF;
        perror("trd_create: pthread_detach");
        return -2;
    }

    pLst->status = ON;

    if (fun_name != NULL)
        strncpy(pLst->name, fun_name, sizeof(pLst->name));

    return 0;
}

/* ==============================================================================================*/
/**
* @brief	:	trd_cancel
*
* @param	:	pLst
*
* @return	:
*/
/* ==============================================================================================*/
int trd_cancel(TRD_LST * pLst)
{
    int ret = -1;
    ret = pthread_cancel(pLst->trd_id);
    return ret;
}

void trd_cleanup(void* arg)
{
    TRD_LST *p = (TRD_LST*)arg;
    trdinfo_off(p, "thead is canceled");

}


/*	$Function	:	trd_lock_init
==	====================================================================================
==	Description	:	thread lock init
==	Argument	:	lock	:	[in]	lock type
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_lock_init(LOCK_t *lock)
{
    pthread_mutex_init(lock, NULL);
}

/*=================================== thread.c end ==================================================*/

