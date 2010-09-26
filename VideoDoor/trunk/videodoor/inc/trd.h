//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//



#ifndef __TRD_H__
#define __TRD_H__

#include "globleinc.h"
#include "support.h"

typedef pthread_t		TRD_t;
typedef sem_t			SEM_t;
typedef pthread_mutex_t	LOCK_t;

int trd_create(TRD_t *trd_id, void *(*func)(void *));

int trd_sem_init(SEM_t *sem, u32 val);
inline void trd_sem_wait(SEM_t *sem);
inline void trd_sem_post(SEM_t *sem);
inline void trd_sem_getvalue(SEM_t *sem, int *val);

inline void trd_lock_init(LOCK_t *lock);
inline void trd_lock_get(LOCK_t *lock);
inline void trd_lock_put(LOCK_t *lock);

#endif	//__TRD_H__

