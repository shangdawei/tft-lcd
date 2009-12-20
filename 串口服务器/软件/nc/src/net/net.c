#include "net.h"
#include <sys/types.h>
#include <sys/socket.h>


int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len)
{
	BYTE *ptr = net_data;


	send(conn_info->fd, ptr, len, MSG_DONTWAIT | MSG_NOSIGNAL);
}

