#ifndef __FILE_H__
#define	__FILE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "system.h"

enum OPEN_FLAGS
{
    MODE_READ      =    0x00,	//!< only read in binary format
    MODE_WRITE,					//!< only write in binary format
    MODE_READWRITE,				//!< read and write dual mode
    MODE_CREATE,				//!< create a file
    MODE_APPEND,				//!< append new content to the existed file tail
    MODE_UPDATE,				//!< Open file for update the contents
    MODE_READ_T,				//!< read text file
    MODE_WRITE_T,				//!< write text file
    MODE_RW_T,                  //!< read and write text file, clean existed contents
    MODE_CREATE_T,				//!< create text file
    MODE_APPEND_T,				//!< append content to text file's tail
    MODE_UPDATE_T,              //!< Open text file for update the contents
};


#define	LOG_PATH "/var/log/nc"
#define USOCKET_FILE	"/tmp/foo.socket"
#define	CONF_PATH	"/etc/nc/"
#define	CFG_FILE	CONF_PATH"sys.cfg"

extern int file_is_existed(char *file_name);
extern FILE *sys_file_open(char *file_name, DWORD flags);
extern void sys_file_close( FILE *fp );
extern int sys_file_read(FILE *file_hd, void *buf, DWORD count);
extern int sys_file_write(FILE *file_hd, void *data, DWORD count);
extern void sys_file_save( void *buf, DWORD len, BYTE *file_name );


#endif

