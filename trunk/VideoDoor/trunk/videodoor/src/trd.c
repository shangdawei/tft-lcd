//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//
#include "trd.h"

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

/*	$Function	:	trd_sem_init
==	====================================================================================
==	Description	:	init thread semaphore
==	Argument	:	sem	:	[in]	semaphore id
				:	val	:	[in]	init value
==	Return		:	0, success; -1, semaphore init error
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
int trd_sem_init(SEM_t *sem, u32 val)
{
    int ret = -1;

    ret = sem_init(sem, 0, val);

    if (ret != 0)
    {
        perror("trd_sem_init: sem_init");
        return -1;
    }

    return 0;
}

/*	$Function	:	trd_sem_wait
==	====================================================================================
==	Description	:	thread semaphore wait
==	Argument	:	sem	:	[in]	semaphore type
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_sem_wait(SEM_t *sem)
{
    sem_wait(sem);
}

/*	$Function	:	trd_sem_post
==	====================================================================================
==	Description	:	thread semaphore post
==	Argument	:	sem	:	[in]	semaphore type
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_sem_post(SEM_t *sem)
{
    sem_post(sem);
}

/*	$Function	:	trd_sem_getvalue
==	====================================================================================
==	Description	:	get semaphore value
==	Argument	:	sem	:	[in]	semaphore type
				;	val	:	[out]	semaphore value
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_sem_getvalue(SEM_t *sem, int *val)
{
    sem_getvalue(sem, val);
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

/*	$Function	:	trd_lock_get
==	====================================================================================
==	Description	:	get lock
==	Argument	:	lock	:	[in]	lock type
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_lock_get(LOCK_t *lock)
{
    pthread_mutex_lock(lock);
}

/*	$Function	:	trd_lock_put
==	====================================================================================
==	Description	:	put lock
==	Argument	:	lock	:	[in]	lock type
==	Return		:	void
==	Modification:	2007/07/06	xuw	create
==	Note		:
==	====================================================================================
*/
inline void trd_lock_put(LOCK_t *lock)
{
    pthread_mutex_unlock(lock);
}

