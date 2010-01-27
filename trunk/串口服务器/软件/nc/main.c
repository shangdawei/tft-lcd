/**
* @file main.c
* @brief	:
* @author 	:xul <xu_liang@dahuatech.com>
* @version 1
* @date	:	 2010-01-27
*/

#include "sys_log.h"
#include "conf.h"

/* ==============================================================================================*/
/**
* @brief	:	main
*
* @param	:	argc
* @param	:	argv[]
*
* @return	:
*/
/* ==============================================================================================*/
int main(int argc, char *argv[])
{

    log_init();
    conf_init();
    sys_log(MOD_MAIN, LOG_MSG, "system start!");

    com_start();
    net_start();

    usocket_start();

    while (1)
    {
        pause();
    }

    return 1;
}

