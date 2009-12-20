
#include "sys_log.h"
#include "conf.h"

int main(int argc, char *argv[])
{
	
	log_init();
	sys_log(MOD_MAIN, LOG_MSG, "system start!");

//	com_start();
//	net_start();
	conf_init();
	usocket_start();
	while(1)
	{
		pause();
	}
	return 1;
}

