/**
* @file sys_log.c
* @brief	:
* @author 	:xul <xu_liang@dahuatech.com>
* @version 1
* @date	:	 2010-01-27
*/
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "sys_log.h"
#include "system.h"
#include "file.h"
#include "thread.h"

const char* g_mod_str[] =
{
    "MOD_MAIN",
    "MOD_FTP",
    "MOD_LOG",
    "MOD_USOCKET",
    "MOD_ERR",
    "MOD_CONF",
    "MOD_COM",
    "MOD_NET",
};
static int g_pfd[2];
static	int g_efd[2];
int log_print(const char* buf);
static void log_thread(void);

/* ==============================================================================================*/
/**
* @brief	:	sys_log
*
* @param	:	module
* @param	:	level
* @param	:	format
* @param	:	...
*
* @return	:
*/
/* ==============================================================================================*/
int sys_log(MOD_LST module, LOG_Lv level, char* format, ...)
{
    char szbuf[1024+256] = "", szcontent[1024] = "";
    va_list ap;
    struct timeval tv;
    struct tm stm;
    va_start(ap, format);
    vsprintf(szcontent, format, ap);
    va_end(ap);

    gettimeofday(&tv, NULL);
    stm = *(localtime(&tv.tv_sec));

    sprintf(szbuf, "%04d-%02d-%02d %02d:%02d:%02d:%06ld---[%s] %s\n",
            stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec, tv.tv_usec,
            g_mod_str[module], szcontent);
    log_print(szbuf);
    return 1;
}

/* ==============================================================================================*/
/**
* @brief	:	perror_to_log
*
* @return	:
*/
/* ==============================================================================================*/
int perror_to_log(void)
{
    int s_err;
    s_err = dup(STDERR_FILENO);

    if (pipe(g_efd) < 0)
    {
        perror("pipe");
        exit(1);
    }

    dup2(g_efd[1], STDERR_FILENO);
    close(g_efd[1]);
    return 1;
}

/* ==============================================================================================*/
/**
* @brief	:	err_log_thread
*/
/* ==============================================================================================*/
static void err_log_thread(void)
{
    char buf[1024];
    char szbuf[1024+256] = "";
    int len;
    struct timeval tv;
    struct tm stm;

    while (1)
    {
        len = read(g_efd[0], buf, sizeof(buf));
        gettimeofday(&tv, NULL);
        stm = *(localtime(&tv.tv_sec));

        sprintf(szbuf, "%04d-%02d-%02d %02d:%02d:%02d:%06ld---[%s] %s",
                stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec, tv.tv_usec,
                g_mod_str[MOD_ERR], buf);

        log_print(szbuf);
    }
}


/* ==============================================================================================*/
/**
* @brief	:	log_print
*
* @param	:	buf
*
* @return	:
*/
/* ==============================================================================================*/
int log_print(const char* buf)
{
    printf("%s", buf);
    write(g_pfd[1], buf, strlen(buf));
    return 1;
}

/* ==============================================================================================*/
/**
* @brief	:	log_init
*
* @return	:
*/
/* ==============================================================================================*/
int log_init(void)
{
    if (pipe(g_pfd) < 0)
    {
        perror("pipe");
        exit(1);
    }

    TRD_t trd_log;
    trd_create(&trd_log, (void *)&log_thread);

    perror_to_log();
    TRD_t trd_elog;
    trd_create(&trd_elog, (void *)&err_log_thread);
}


/* ==============================================================================================*/
/**
* @brief	:	log_thread
*/
/* ==============================================================================================*/
static void log_thread(void)
{
    char buf[2048];
    int len;
    char log_file[128] = "";
    struct timeval tv;
    struct tm stm;
    sys_log(MOD_LOG, LOG_MSG, "%s start!", __FUNCTION__);

    while (1)
    {
        len = read(g_pfd[0], buf, sizeof(buf));
        memset(log_file, 0, sizeof(log_file));

        gettimeofday(&tv, NULL);
        stm = *(localtime(&tv.tv_sec));

        sprintf(log_file, "%s/%04d-%02d-%02d.log", LOG_PATH, stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday);
        int fd = open(log_file, O_CREAT | O_WRONLY | O_APPEND);

        if (fd > 0)
        {
            if (write(fd, buf, len) < 0)
            {
                perror("write");
                exit(1);
            }

            close(fd);
        }
        else
        {
            perror("log thread open");
            exit(1);
        }
    }
}

