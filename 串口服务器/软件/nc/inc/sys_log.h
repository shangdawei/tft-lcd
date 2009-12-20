#ifndef __SYS_LOG_H__
#define	__SYS_LOG_H__

typedef enum tagLOG_Lv
{
	LOG_MSG,
	LOG_WARN,
	LOG_ERR,
}LOG_Lv;


typedef enum tagMOD_LST
{
	MOD_MAIN,
	MOD_FTP,
	MOD_LOG,
	MOD_USOCKET,
	MOD_ERR,
	MOD_CONF,
}MOD_LST;



extern int log_init(void);
extern int sys_log(MOD_LST module, LOG_Lv level, char* format,...);

#endif

