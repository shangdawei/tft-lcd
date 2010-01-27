/*
**
** thread.h
**
*/
#ifndef	_TRD_H_
#define	_TRD_H_
#include <pthread.h>
#include <semaphore.h>
#include "system.h"
#include "conf.h"

typedef pthread_t		TRD_t;
typedef sem_t			SEM_t;
typedef pthread_mutex_t	LOCK_t;

#define trd_cleanup_push(a,b)   pthread_cleanup_push(a,b)
#define trd_cleanup_pop()       pthread_cleanup_pop(0)
// for thread safe
#define TS_LOCK(lock)	pthread_cleanup_push((void(*)(void*))pthread_mutex_unlock, (void*)lock );	pthread_mutex_lock( lock )

#define TS_UNLOCK(lock)	pthread_cleanup_pop( 1 )

//#define	TRD_NUM		30

typedef	enum
{
    TRD_CAP	= 0x00,
    TRD_LOG,
    TRD_TIMER,
    TRD_CONSOLE,
    TRD_PICUP,
    TRD_RTPICUP,

    TRD_DSP,


} TRD_ID;

typedef	struct	tagTRD_LST
{
    DWORD			id;
    pthread_t 		trd_id;
    BYTE			name[16];
    BYTE			status;
    BYTE			msg[63];
} TRD_LST;



extern void trdinfo_on(TRD_LST* ptrdlst);
extern void trdinfo_off(TRD_LST* ptrdlst, char *msg);
extern int trd_create(TRD_t *trd_id, void *(*func)(void *));
extern inline void trd_lock_init(LOCK_t *lock);
extern int trd_create2(TRD_LST * pLst, void *(*func)(void *), void* arg, char * fun_name);
extern int trd_cancel(TRD_LST * pLst);
extern void trd_cleanup(void* arg);
#endif //_TRD_H_
/*=================================== thread.h end ==================================================*/


