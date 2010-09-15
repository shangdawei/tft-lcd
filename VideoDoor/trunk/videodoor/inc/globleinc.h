//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#ifndef _VD_INC_H
#define _VD_INC_H

/*! standard C library */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

/*! Linux system calls */
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/mount.h>
#include <sys/select.h>
#include <sys/vfs.h>
#include <sys/time.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/io.h>
#include <sys/sendfile.h>

/*! other interfaces */
#include <time.h>
#include <string.h>
#include <memory.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <termios.h> 
#include <dirent.h>
#include <crypt.h>
#include <limits.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>

/*! linux kernel supported interfaces */
#include <linux/hdreg.h>

// for thread safe 
#define TS_LOCK(lock)	pthread_cleanup_push((void(*)(void*))pthread_mutex_unlock, (void*)lock );	pthread_mutex_lock( lock )

#define TS_UNLOCK(lock)	pthread_cleanup_pop( 1 )

#endif /* _VD_INC_H end */
/*====================================== End of globleinc.h =====================================*/

