//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "console.h"
#include "print.h"

#define PARTAB_SZ		(sizeof(ParseTable) / sizeof(PAR_TAB))


static CONSOLE console;
static int console_fd = -1;
static int sock_listen = -1;
static int sock_console = -1;
static int s_out = -1;
static int s_err = -1;
static struct termios orig_termios;

void console_recover();
static void console_parse(char ch);

void console_help();
void console_system_cmd();
void console_exit_cmd();

PAR_TAB ParseTable[] = {
    { LVL_USR, "help",    console_help,     "Show System Command Supported"},
	{ LVL_USR, "!",       console_system_cmd,"Execute Linux System Command"},
	{ LVL_USR, "exit",    console_exit_cmd,"Teletn Logout"},

};


//////////////////////////////////////////////////////////////////////////
///
///     console_proc
///		@author xuliang<xu_liang@dahuatech.com>
///		@date	2010年09月15日
//////////////////////////////////////////////////////////////////////////
static void console_proc()
{
    int flag = 1;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    int clilen;
    fd_set  fds;
    int ret;
    char address[16];
    int max_fd;
    struct timeval tv;

    sys_print(FUNC, INFO,"console_procp start = %d\n", getpid());
#ifdef _CONSOLE
    console_fd = open(DEV_COM, O_RDONLY);

    if (console_fd == -1)
    {
        perror("stdin open!");
    }
    else
    {
        if (tcgetattr(console_fd, &orig_termios) < 0)
        {
            perror("tcgetattr:");
        }
    }

#endif
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_listen == -1)
    {
        sys_print(FUNC, ERROR, "console_tcp_start socket() error!\n");
        return;
    }

    if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, \
                   &flag, sizeof(flag)) == -1)
    {
        sys_print(FUNC, ERROR, "console_tcp_start setsocketopt() error!\n");
        return;
    }

    bzero(&servaddr, sizeof(struct sockaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(CON_LISTEN_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_listen, (struct sockaddr *)&servaddr, \
             sizeof(struct sockaddr)) == -1)
    {
        sys_print(FUNC, ERROR, "console_tcp_start bind() error!\n");
		close(sock_listen);
        return;
    }

    listen(sock_listen, 2);
    signal(SIGPIPE, SIG_IGN);
    s_out = dup(STDOUT_FILENO);
    s_err = dup(STDERR_FILENO);

    while (1)
    {
        FD_ZERO(&fds);

        tv.tv_sec = 0;
        tv.tv_usec = 200000;

        if (sock_console == -1)
        {
            if (console_fd > 0)
            {
                FD_SET(console_fd, &fds);
            }

            FD_SET(sock_listen, &fds);
            max_fd = sock_listen > console_fd ? sock_listen : console_fd;
            max_fd ++;
        }
        else
        {
            FD_SET(sock_console, &fds);
            max_fd = sock_console + 1;
        }

        ret = select(max_fd, &fds, NULL, NULL, &tv);

        if (errno == EINTR || ret == 0)
        {
            continue;
        }

        if (ret < 0)
        {
            perror("console.select:");
            return;
        }

        if (FD_ISSET(sock_listen, &fds))
        {
            clilen = sizeof(struct sockaddr);
            sock_console = accept(sock_listen, (struct sockaddr *) & cliaddr, \
                                  &clilen);

            if (sock_console == -1)
            {
                perror("consolet.accept:");
                continue;
            }

            sys_ip2str(cliaddr.sin_addr.s_addr, address);
            sys_print(FUNC, INFO, "Remote Console Connected From[%s]\n", address);
            dup2(sock_console, STDOUT_FILENO);
            dup2(sock_console, STDERR_FILENO);
            sys_print(FUNC, INFO, "Remote Console Connected From[%s]\n", address);
            console.idletime = 0;

            /*current user logout!*/
            console.status = CON_USRNAME;
            console.rxlen = 0;
            memset(console.rxbuf, 0, RXBUF_SZ);
            printf("\f\nUSERNAME:");
            continue;
        }

        if (sock_console > 0 && FD_ISSET(sock_console, &fds))
        {
            console_read(sock_console);
        }

        if (console_fd > 0 && FD_ISSET(console_fd, &fds))
        {
            console_read(console_fd);
        }

        fflush(stdout);
    }
}


int console_read(int fd)
{
	char buf[16];
	BYTE nread;
	BYTE i;
	unsigned char ch = 0;

	nread = read(fd, buf, sizeof(buf));
	if(nread > 0)
	{
		FOR(i, nread)
		{
			if( (buf[i] == ASCII_CR) && (ch == ASCII_LF))
				continue;
			console_parse(buf[i]);
			ch = buf[i];
		}
		console.idletime = 0;
	}
	else
	{
		if(sock_console != -1)
		{
			close(sock_console);
			console_recover();
		}
	}
}

void console_recover()
{

	if(sock_console != -1)
	{
		fflush(stdin);
		fflush(stdout);
		fflush(stderr);
		close(sock_console);
		if(s_err != -1)
		{
			dup2(s_out, STDOUT_FILENO);
			dup2(s_err, STDERR_FILENO);
		}
	}
	if(console.status != CON_OFF && console.status != CON_USRNAME)
	{
		printf("\nUsr Exit!");
		printf("\f\nUSERNAME:");
	}
	console.rxlen = 0;
	console.status = CON_USRNAME;
	sock_console = -1;
	console.idletime = 0;
}

void console_echo_enable(BOOL flag)
{
	if(console_fd < 0)
	{
		return;
	}
	if(flag)
	{
		orig_termios.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);
	}
	else
	{
		orig_termios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
	}
	if(tcsetattr(console_fd, TCSANOW, &orig_termios) < 0)
	{
		perror("tcsetattr"); 
	} 
	return;
}


void console_echo(char ch)
{
	if(sock_console != -1)
	{
		printf("%c",ch);
	}
	return;
}


void console_system_cmd()
{
	char cmdbuf[RXBUF_SZ];
	strcpy(cmdbuf, &console.rxbuf[1]);
	system(cmdbuf);
	return;
}

void console_exit_cmd()
{
	console_recover();

}
void console_cmd_parse()
{
	int i, j, k;

	console.params = 0;
	i = -1;
	j = 0;
	k = 0;
	while (console.rxbuf[k]) {
		if (k >= RXBUF_SZ)
			break;
		if (console.rxbuf[k] == ' ' || console.rxbuf[k] == ASCII_TAB) {
			if (i >= 0 && j > 0)
				console.partab[i][j] = '\0';
			j = 0;
			k ++;
			continue;
		}
		if (j == 0) {
			if (++i >= 8) break;
			console.params = i;
		}
		console.partab[i][j++] = console.rxbuf[k];
		if (j >= 32) {
			while (console.rxbuf[k]) {
				if (k >= RXBUF_SZ)
					break;
				if (console.rxbuf[k] == ' ') break;
				k ++;
			}
		}
		k ++;
	}
	if(j > 0)
	{
		console.partab[i][j] = '\0';
	}

	if(console.partab[0][0] == '!')
	{
		console_system_cmd();
		return;
	}
	FOR (i, PARTAB_SZ)
	{
		if (strlen(console.partab[0]) < 3) {
			printf("[%s]: No this command, please type 'help' for details.\n", console.partab[0]);
			return;
		}
		if (strncmp(console.partab[0], ParseTable[i].ParseStr, strlen(console.partab[0])) == 0)
			break;
	}
	if (i >= PARTAB_SZ) {
		printf("[%s]: No this command, please type 'help' for details.\n", console.partab[0]);
		return;
	}
	ParseTable[i].ParseProc();
	return;
}



void console_line_cmd(char ch)
{
	switch (ch) {
	case ASCII_CR:
	case ASCII_LF:
		if (console.rxlen == 0) {
			console_echo('\n');
			printf("DEBUG>");
		} else {
			console.rxbuf[console.rxlen ++] = '\0';
			console_cmd_parse();
			console.rxlen = 0;
			if (console.status == CON_LINE)
			{
				printf("\nDEBUG>");
			}
		}
		break;
	case ASCII_BACK:
		if (console.rxlen > 0) {
			console.rxlen --;
			printf("\b \b");
		}
		break;
	default:
		//console.rxbuf[console.rxlen++] = tolower(ch);
		console.rxbuf[console.rxlen++] = ch;
		if (console.rxlen >= (RXBUF_SZ - 2))
			console.rxlen = 0;
		console_echo(ch);
		break;
	}
}



static void console_parse(char ch)
{
	char *psw;
	int i;
	char status = 0;
	static BYTE usrname[NAME_LEN];
	USR_INFO *usr_info = NULL;

	switch (console.status)
	{
	case CON_OFF:
		printf("\f\nUSERNAME:");
		console.status = CON_USRNAME;
		console.rxlen = 0;
		break;
	case CON_USRNAME:
		switch (ch) {
		case ASCII_CR:
		case ASCII_LF:
			if(console.rxlen == 0)
			{
				printf("\f\nUSERNAME:");
				console.status = CON_USRNAME;
				break;
			}
			console.rxbuf[console.rxlen++] = '\0';
			strncpy(usrname, console.rxbuf, NAME_LEN);
			if(strcmp(console.rxbuf, "superits") == 0)
			{
				console.rxlen = 0;
				console.status = CON_LINE;
				console.right = S_ADMIN;
				console.uid = 0;
				memcpy(console.name, console.rxbuf, NAME_LEN);
				printf("\f\n");
				printf("\nDEBUG>");
				break;
			}
			else
			{
				psw = console.rxbuf;
/*
				if(sock_console == -1)
				{
					psw = getpass("PASSWORD:");
					if(user_login(usrname, psw) == S_ACTIVE)
					{
						usr_info = user_info_get(usrname);
						if(usr_info == NULL)
						{
							printf("System Error!User Not Exist\n");
						}
						else
						{
							console.rxlen = 0;
							console.uid = usr_info->usr_id;
							console.right = usr_info->usr_auth;
							console.status = CON_LINE;
							memcpy(console.name, usrname, NAME_LEN);
							printf("\f\n");
							printf("\nDEBUG>");
							break;
						}
					}
					printf("\nLOGIN FAILURE!");
					printf("\nUSERNAME:");
					console.rxlen = 0;
					console.status = CON_USRNAME;
				}
				else
*/
				{
					printf("\nPASSWORD:");
					console.rxlen = 0;
					console.status = CON_PASSWD;
					console_echo_enable(FALSE);
				}
			}
			break;
		case ASCII_BACK:
			if (console.rxlen > 0) {
				console.rxlen --;
				printf("\b \b");
			}
			break;
		default:
			console.rxbuf[console.rxlen++] = ch;
			if (console.rxlen > (RXBUF_SZ - 8))
				console.rxlen = 0;
			console_echo(ch);
			break;
		}
		break;
	case CON_PASSWD:
/*
		if(sock_console == -1)
		{
			console.rxlen = 0;
			console.status = CON_USRNAME;
			break;
		}
*/
		switch (ch) {
		case ASCII_CR:
		case ASCII_LF:
			console.rxbuf[console.rxlen++] = '\0';
			psw = console.rxbuf;
			console_echo_enable(TRUE);
            /*
			if(user_login(usrname, psw) == S_ACTIVE)
			{
				usr_info = user_info_get(usrname);
				if(usr_info != NULL)
				{
					console.rxlen = 0;
					console.uid = usr_info->usr_id;
					console.right = usr_info->usr_auth;
					console.status = CON_LINE;
					memcpy(console.name, usrname, NAME_LEN);
					printf("\f\n");
					printf("\nDEBUG>");
					break;
				}
			}*/
			printf("\nLOGIN FAILURE!");
			printf("\nUSERNAME:");
			console.rxlen = 0;
			console.status = CON_USRNAME;
			break;
		case ASCII_BACK:
			if (console.rxlen > 0) {
				console.rxlen --;
				printf("\b \b");
			}
			break;
		default:
			console.rxbuf[console.rxlen++] = ch;
			if (console.rxlen > (RXBUF_SZ - 8))
				console.rxlen = 0;
			printf("*");
			break;
		}
		break;
	case CON_LINE:
		console_line_cmd(ch);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
///
///     console_init
///		@author xuliang<xu_liang@dahuatech.com>
///		@date	2010年09月15日
//////////////////////////////////////////////////////////////////////////
void console_init(void)
{
    pthread_t trd_console;
    console.rxlen  = 0;
    console.params = 0;
    console.status = CON_OFF;
    console.idletime = 0;
    trd_create(&trd_console, (void *)&console_proc);
}

void console_help()
{
	BYTE i;

	printf("\n");
	printf("		+---------+-------------------------------------------------------+\n");
	printf("		| Command | Description                                           |\n");
	printf("		+---------+-------------------------------------------------------+\n");
	FOR (i, PARTAB_SZ)
	{
		printf("		|%-8s | %-54s|\n", ParseTable[i].ParseStr, ParseTable[i].HelpMsg);
		printf("		+---------+-------------------------------------------------------+\n");
	}
}

