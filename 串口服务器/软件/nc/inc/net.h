#ifndef	__NET_H__
#define	__NET_H__

#include "system.h"
#include "usocket.h"

extern int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len);

#endif

