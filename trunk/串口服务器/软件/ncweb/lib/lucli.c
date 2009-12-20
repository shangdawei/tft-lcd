#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "lucli.h"

#define CLI_PATH    "/var/tmp/"      /* +5 for pid = 14 chars */
#define CLI_PERM    S_IRWXU          /* rwx for user only */
#define offsetof(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)
/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */
int
cli_conn(const char *name)
{
    int                fd, len, err, rval;
    struct sockaddr_un un;

    /* create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return(-1);

    /* fill socket address structure with our address */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "%s%05d", CLI_PATH, getpid());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    unlink(un.sun_path);        /* in case it already exists */
    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
        rval = -2;
        goto errout;
    }
    if (chmod(un.sun_path, CLI_PERM) < 0) {
        rval = -3;
        goto errout;
    }
    /* fill socket address structure with server's address */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if (connect(fd, (struct sockaddr *)&un, len) < 0) {
        rval = -4;
        goto errout;
    }
    return(fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return(rval);
}


DWORD sys_str2ip(BYTE *str)
{
	BYTE *p = str;
	BYTE cIp;
	DWORD ip = 0;
	signed char j = 0;

	cIp = (BYTE)atoi(p);
	ip += (((DWORD)(cIp)) << (j * 8)); 
	j++;

	while ((*p != 0) && (j >= 0))
	{
		if (*p != '.')
		{
			p++;
			continue;
		}

		cIp = (BYTE)atoi(++p);
		
		ip += (((DWORD)(cIp)) << (j * 8));  
		j++;
	}
	
	return ip;
}



void sys_ip2str(DWORD ip, BYTE *str)
{
	BYTE i;
	BYTE cIp[4];
	DWORD iIp = ip;
					
	for (i = 0; i < 4; i++)
	{
		cIp[i] = (BYTE)( (iIp>>(i*8)) & (0xff) );
	}
	sprintf(str, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
}

int net_conn_send(int fd, void *net_data, DWORD len)
{
	BYTE *ptr = net_data;


	send(fd, ptr, len, MSG_DONTWAIT | MSG_NOSIGNAL);
}


typedef struct tagITSIP
{
	BYTE itsip_head;     //!< head, 0xff
	BYTE itsip_cmd;      //!< command
	BYTE itsip_ver;      //!< protocol version
	BYTE itsip_thl;      //!< the head length
	WORD itsip_extlen;   //!< external data length
	BYTE itsip_dev_t;    //!< Device types
	BYTE itsip_rsvd;     //!< Reserved byte
	BYTE itsip_data[24]; //!< data
} ITSIP, *p_ITSIP;

typedef struct tagITSIP_PKT
{
	ITSIP head;				//!< ITSIP head
	BYTE data[MAX_LENGTH];	//!< external data, 1024 bytes
} ITSIP_PACKET, *p_ITSIP_PACKET;


inline void itsip_pack(int cmd, int extlen, void *data, ITSIP_PACKET *its_pkt)
{
//	memset(its_pkt, 0, sizeof(ITSIP_PACKET));
	its_pkt->head.itsip_head = ITS_HEAD;
	its_pkt->head.itsip_cmd = cmd;
	its_pkt->head.itsip_ver = ITSIP_VERSION;
	its_pkt->head.itsip_thl = ITSIP_THLEN;
	its_pkt->head.itsip_extlen = extlen;
	if(extlen > 0 || data != NULL)
	{
		memcpy(its_pkt->data, data, extlen);
	}
}

int  readn( int fd, char *ptr, int nbytes)
{
	int  nleft, nread ;

	nleft = nbytes ;
	while ( nleft > 0 )
	{
		nread = read( fd, ptr, nleft) ;
		if ( nread < 0 )
			return ( nread) ;
		if ( nread == 0 )
			break ;
		nleft = nleft - nread ;
		ptr = ptr + nread ;
	}
	return ( nbytes - nleft);
}


int get_net(int fd, CONFIG_NET *pnet)
{
	ITSIP_PACKET conf_pkt;
	int len;
	itsip_pack(ITS_VER_QUERY, 0, NULL, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_NET;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP));
//	printf("11111\n");	
	memset(&conf_pkt, 0, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP_PACKET));
//	printf("22222\n");	
	if(len != sizeof(ITSIP_PACKET))
	{
		return 0;
	}
	memcpy(pnet, conf_pkt.data, sizeof(CONFIG_NET));
	return 1;
}

int set_net(int fd, CONFIG_NET *pnet)
{
	ITSIP_PACKET conf_pkt;
	int len;
//	printf("[%d][%d][%d]\n", pnet->dev_ip, pnet->dev_nm, pnet->dev_gw);
	memcpy(conf_pkt.data, (char	*)pnet, sizeof(CONFIG_NET));
	itsip_pack(ITS_VER_SET, sizeof(CONFIG_NET), conf_pkt.data, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_NET;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP) + sizeof(CONFIG_NET));
//	printf("11111\n");	
	memset(&conf_pkt, 1, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP));
//	printf("22222\n");	
	if(len != sizeof(ITSIP))
	{
		return 0;
	}
	if(conf_pkt.head.itsip_data[1] == 0)
		return 1;

	return 0;

}

int get_com(int fd, CONFIG_COM *pcom)
{
	ITSIP_PACKET conf_pkt;
	int len;
	itsip_pack(ITS_VER_QUERY, 0, NULL, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_COM;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP));

	memset(&conf_pkt, 0, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP_PACKET));

	if(len != sizeof(ITSIP_PACKET))
	{
		return 0;
	}
	memcpy(pcom, conf_pkt.data, sizeof(CONFIG_COM) * MAX_NC_PORT);
	return 1;
}

int set_com(int fd, CONFIG_COM *pcom)
{
	ITSIP_PACKET conf_pkt;
	int len;

	memcpy(conf_pkt.data, (char	*)pcom, sizeof(CONFIG_COM) * MAX_NC_PORT);
	itsip_pack(ITS_VER_SET, sizeof(CONFIG_COM) * MAX_NC_PORT, conf_pkt.data, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_COM;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP) + sizeof(CONFIG_COM) * MAX_NC_PORT);

	memset(&conf_pkt, 1, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP));

	if(len != sizeof(ITSIP))
	{
		return 0;
	}
	if(conf_pkt.head.itsip_data[1] == 0)
		return 1;

	return 0;

}


int get_wknet(int fd, CONFIG_WKNET *pcom)
{
	ITSIP_PACKET conf_pkt;
	int len;
	itsip_pack(ITS_VER_QUERY, 0, NULL, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_WKNET;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP));

	memset(&conf_pkt, 0, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP_PACKET));

	if(len != sizeof(ITSIP_PACKET))
	{
		return 0;
	}
	memcpy(pcom, conf_pkt.data, sizeof(CONFIG_WKNET) * MAX_NC_PORT);
	return 1;
}


int set_wknet(int fd, CONFIG_WKNET *pcom)
{
	ITSIP_PACKET conf_pkt;
	int len;

	memcpy(conf_pkt.data, (char	*)pcom, sizeof(CONFIG_WKNET) * MAX_NC_PORT);
	itsip_pack(ITS_VER_SET, sizeof(CONFIG_WKNET) * MAX_NC_PORT, conf_pkt.data, &conf_pkt);
	conf_pkt.head.itsip_data[0] = CONF_WKNET;
	net_conn_send(fd, &conf_pkt, sizeof(ITSIP) + sizeof(CONFIG_WKNET) * MAX_NC_PORT);

	memset(&conf_pkt, 1, sizeof(ITSIP_PACKET));
	len = readn(fd, (char*)&conf_pkt, sizeof(ITSIP));

	if(len != sizeof(ITSIP))
	{
		return 0;
	}
	if(conf_pkt.head.itsip_data[1] == 0)
		return 1;

	return 0;
}


#if 0
int
main(void)
{
    int fd;
	char buf[32]="";
        
    fd = cli_conn("/tmp/foo.socket");
		printf("%d\n", fd);

	while(1)
	{
		scanf("%s", buf);
	//	printf("it is [%s]\n", buf);
		ITSIP_PACKET conf_pkt;
		itsip_pack(ITS_VER_QUERY, 0, NULL, &conf_pkt);
		conf_pkt.head.itsip_data[0] = 0x01;
		net_conn_send(fd, &conf_pkt, sizeof(ITSIP_PACKET));
	}
	while(1)
	{
		write(fd, buf, sizeof(buf));
		buf[1]++;
		if(buf[1] > '9')
			buf[1] = '1';
		sleep(1);
	}
		
		
		
    exit(0);
}
#endif

