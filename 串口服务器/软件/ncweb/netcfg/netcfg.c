#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lucli.h"

void getpar(char *src,char *name); 
void ShowForm();
void getformstrchar(char *name, char *buf, int size);
int getformstrval(char *name);
void getmode();
void gettcpmode();
void getcr();
void getlf();
void getmax_num();
void getatt();
void getreg();
void getsrv();
void getlport();
void getisnull();
void getprotocol();
void getip();
void getport();
void getconn();
void getdisconn();
void gettime_out();

int getmsg(CONFIG_WKNET* p);
int setmsg(CONFIG_WKNET* p);


void print_css(void);

CONFIG_WKNET wknet_cfg[MAX_NC_PORT];


char ipaddr[16]="";
char netmask[16]="";
char gateway[16]="";


static int id=0;

void print_backgrounf(void)
{
	fprintf(cgiOut, "<style type=\"text/css\">\n");
	fprintf(cgiOut, "<!--\n");
	fprintf(cgiOut, "body {\n");
	fprintf(cgiOut, "background-image: url(../images/blue-tile.png);\n");
	fprintf(cgiOut, "background-repeat: repeat;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "-->\n");
	fprintf(cgiOut, "</style>\n");
}
int cgiMain() 
{
	id = atoi(cgiQueryString);
	if(id > 0)
		id --;
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html;charset=gb2312");
	/* Top of the page */
	fprintf(cgiOut, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>NC600 Web Server</TITLE>\n");
	print_css();
	print_backgrounf();
	fprintf(cgiOut,	"</HEAD><BODY>\n");
	getmsg(wknet_cfg);
	if(cgiFormSubmitClicked("host") == cgiFormSuccess)
	{
		getmode();
		gettcpmode();
 		getcr();
 		getlf();
 		getmax_num();
 		getatt();
 		getreg();
 		getsrv();
 		getlport();
 		getisnull();
		getprotocol();
 		getip();
 		getport();
 		getconn();
 		getdisconn();
 		gettime_out();
		setmsg(wknet_cfg);
		fprintf(cgiOut, "<H1>请重启相应端口&nbsp;&nbsp;</H1>\n");

	}
	ShowForm();
	fprintf(cgiOut, "</BODY></HTML>\n");
	
	return 0;
}

int getmsg(CONFIG_WKNET* p)
{
	int fd;
	
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}
	if(!get_wknet(fd, p))
	{
		close(fd);
		return 3;
	}

	close(fd);
	return 1;
}

int setmsg(CONFIG_WKNET* p)
{
	int fd;
	int i;
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}

	
	if(!set_wknet(fd, p))
	{
		close(fd);
		return 3;
	}

	close(fd);
	return 1;

}


char* modestr[] = 
 {
	"---------------",
	"TCP/UDP Socket",
 };

char* tcp_modestr[] = 
 {
	"---------------",
	"raw",
	"telnet",
 };

char* crstr[] = 
 {
	"---------------",
	"CR",
	"LF",
 };

char* max_numtr[] = 
 {
	"---------------",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
 };
char* str[] = 
 {
	"---------------",
	"no",
	"yes",
 };


char* att_str[] = 
 {
	"---------------",
	"none",
 };

char* protocol_str[] = 
 {
	"---------------",
	"TCP client",
	"TCP server",
	"UDP",
 };

char* conn_str[] = 
 {
	"---------------",
	"always",
 };
char* dis_conn_str[] = 
 {
	"---------------",
	"none",
 };

void ShowForm()
{
 int i ,j;
 fprintf(cgiOut, "<div id=\"formwrapper\">\n");
 fprintf(cgiOut, "	<h3>端口 %d 模式配置</h3>\n", id + 1);
 fprintf(cgiOut, "	<form action=\"");
 fprintf(cgiOut, "\" method=\"post\" name=\"wknet\" id=\"wknet\">\n");
 fprintf(cgiOut, " \n");
 fprintf(cgiOut, "	\n");
 fprintf(cgiOut, "	  <fieldset>\n");
 fprintf(cgiOut, " <table width=\"85%%\">\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"Name\">工作模式</label>\n");
 fprintf(cgiOut, "		<select name=\"mode\" width=\"100\">\n");
 for(i = 0; i < sizeof(modestr) / sizeof(modestr[0]); i++)
 {
	if(wknet_cfg[id].mode == i)
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", modestr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", modestr[i]);
 }
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td></td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">TCP数据模式</label>\n");
 fprintf(cgiOut, "		<select  name=\"tcp_mode\"  width=\"100\">\n");
 for(i = 0; i < sizeof(tcp_modestr) / sizeof(tcp_modestr[0]); i++)
 {
	 if(wknet_cfg[id].tcp_mode == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", tcp_modestr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", tcp_modestr[i]);
 }
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><label for=\"password\">本地端口</label>\n");
 fprintf(cgiOut, "		<input name=\"lport\" size=\"18\" maxlength=\"9\" value=\"%d\" /></td>\n", wknet_cfg[id].lport);
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><label for=\"password\">CR解释为</label>\n");
 fprintf(cgiOut, "		<select name=\"CR\" width=\"100\">\n");
 for(i = 0; i < sizeof(crstr) / sizeof(crstr[0]); i++)
 {
	 if(wknet_cfg[id].CR == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", crstr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", crstr[i]);
 }
 fprintf(cgiOut, "	   </select></td>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">LF解释为</label>\n");
 fprintf(cgiOut, "		<select name=\"LF\" width=\"100\">\n");
 for(i = 0; i < sizeof(crstr) / sizeof(crstr[0]); i++)
 {
	 if(wknet_cfg[id].LF == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", crstr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", crstr[i]);
 }
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">会话数</label>\n");
 fprintf(cgiOut, "		<select name=\"max_num\" width=\"100\">\n");
 for(i = 0; i < sizeof(max_numtr) / sizeof(max_numtr[0]); i++)
 {
	 if(wknet_cfg[id].max_num == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", max_numtr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", max_numtr[i]);
 }

 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">忽略NULL字符</label>\n");
 fprintf(cgiOut, "		<select name=\"is_null\" width=\"100\">\n");
 for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
 {
	 if(wknet_cfg[id].is_null == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", str[i]);
 }

 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">认证</label>\n");
 fprintf(cgiOut, "		<select name=\"attestation\" width=\"100\">\n");
 for(i = 0; i < sizeof(att_str) / sizeof(att_str[0]); i++)
 {
	 if(wknet_cfg[id].attestation == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", att_str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", att_str[i]);
 }

 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		 <br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">认证提示</label>\n");
 fprintf(cgiOut, "		<select name=\"reg\" width=\"100\">\n");
 for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
 {
	 if(wknet_cfg[id].reg == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", str[i]);
 }

 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td><div>\n");
 fprintf(cgiOut, "		<label for=\"password\">SERVER优先</label>\n");
 fprintf(cgiOut, "		<select name=\"srv_1st\" width=\"100\">\n");
 for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
 {
	 if(wknet_cfg[id].srv_1st == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", str[i]);
 }

 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td>&nbsp;</td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td>&nbsp;</td>\n");
 fprintf(cgiOut, "	  <td>&nbsp;</td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td>&nbsp;</td>\n");
 fprintf(cgiOut, "	  <td>&nbsp;</td>\n");
 fprintf(cgiOut, "	</tr>\n");
 fprintf(cgiOut, "</table>\n");
 fprintf(cgiOut, "	  <table width=\"85%%\">\n");
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">会话\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">协议\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">对端主机\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">对端端口\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">发起连接\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">断开连接\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td >\n");
 fprintf(cgiOut, "		<div align=\"center\">空闲时间\n");
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	</tr>\n");
 for(j = 0; j < MAX_SESSION; j++) 
 	{
 fprintf(cgiOut, "	<tr>\n");
 fprintf(cgiOut, "	  <td>\n");
 fprintf(cgiOut, "		<div align=\"center\">%d\n", j + 1);
 fprintf(cgiOut, "		</div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 fprintf(cgiOut, "		<select name=\"protocol_str_%d\" width=\"100\">\n", j+1);
 for(i = 0; i < sizeof(protocol_str) / sizeof(protocol_str[0]); i++)
 {
	 if(wknet_cfg[id].session[j].protocol == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", protocol_str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", protocol_str[i]);
 }

 fprintf(cgiOut, "		</select>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 sys_ip2str(wknet_cfg[id].session[j].ip, ipaddr);
 fprintf(cgiOut, "		<input type=\"text\" name=\"ip_%d\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", j+1, ipaddr);
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 fprintf(cgiOut, "		<input type=\"text\" name=\"port_%d\" id=\"password\" size=\"8\" maxlength=\"8\" value=\"%d\"/>\n", j+1, wknet_cfg[id].session[j].port);
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 fprintf(cgiOut, "		<select name=\"conn_%d\" width=\"100\">\n", j+1);
 for(i = 0; i < sizeof(conn_str) / sizeof(conn_str[0]); i++)
 {
	 if(wknet_cfg[id].session[j].conn == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", conn_str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", conn_str[i]);
 }

 fprintf(cgiOut, "		</select>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 fprintf(cgiOut, "		<select name=\"dis_conn_%d\" width=\"100\">\n", j+1);
 for(i = 0; i < sizeof(dis_conn_str) / sizeof(dis_conn_str[0]); i++)
 {
	 if(wknet_cfg[id].session[j].dis_conn == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", dis_conn_str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", dis_conn_str[i]);
 }

 fprintf(cgiOut, "		</select>\n");
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	  <td><div align=\"center\">\n");
 fprintf(cgiOut, "		<input type=\"text\" name=\"time_out_%d\" size=\"8\" maxlength=\"8\" value=\"%d\"/>\n", j+1, wknet_cfg[id].session[j].time_out);
 fprintf(cgiOut, "	  </div></td>\n");
 fprintf(cgiOut, "	</tr>\n");

 	}
 fprintf(cgiOut, "</table>\n");
 fprintf(cgiOut, "	  <div class=\"cookiechk\">\n");
 fprintf(cgiOut, "		<label>  \n");
 fprintf(cgiOut, "	  \n");
 fprintf(cgiOut, "		</label>\n");
 fprintf(cgiOut, "	   <label>	\n");
 fprintf(cgiOut, "	  \n");
 fprintf(cgiOut, "		</label>\n");
 fprintf(cgiOut, "	   <label>	\n");
 fprintf(cgiOut, "	  \n");
 fprintf(cgiOut, "		</label>\n");
 fprintf(cgiOut, "		<input name=\"host\" type=\"submit\" class=\"buttom\" value=\"提交\" />\n");
 fprintf(cgiOut, "		 \n");
 fprintf(cgiOut, "		 <input type=\"checkbox\" name=\"CookieYN\" id=\"CookieYN\" value=\"1\" />\n");
 fprintf(cgiOut, "		<a href=\"#\" title=\"选择是否记录您的信息\">设置应用到所有串口</a>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	  <div class=\"forgotpass\"></div>\n");
 fprintf(cgiOut, "	  </fieldset>\n");
 fprintf(cgiOut, "	</form>\n");
 fprintf(cgiOut, "	<br/>\n");
 fprintf(cgiOut, "	<h3>&nbsp;</h3>\n");
 fprintf(cgiOut, "</div>\n");


}


void getpar(char *src,char *name) {
	
	cgiFormStringNoNewlines(src, name, 16);
//	fprintf(cgiOut, "%s: ", src);
//	cgiHtmlEscape(name);
//	fprintf(cgiOut, "<BR>\n");
}

void Passwd(char *passwd) {

	cgiFormStringNoNewlines("passwd", passwd, 81);
	fprintf(cgiOut, "Passwd: ");
	cgiHtmlEscape(passwd);
	fprintf(cgiOut, "<BR>\n");
}


void getformstrchar(char *name, char *buf, int size)
{
	cgiFormStringNoNewlines(name, buf, size);
}


int getformstrval(char *name)
{
	char buf[64];
	cgiFormStringNoNewlines(name, buf, 64);
	return atoi(buf);
}

void getmode() {
	
	int i;
	cgiFormSelectSingle("mode", modestr, sizeof(modestr)/sizeof(modestr[0]), &i, 0);
	wknet_cfg[id].mode = i;
}

void gettcpmode() {
	
	int i;
	cgiFormSelectSingle("tcp_mode", tcp_modestr, sizeof(tcp_modestr)/sizeof(tcp_modestr[0]), &i, 0);
	wknet_cfg[id].tcp_mode = i;
}

void getcr() {
	
	int i;
	cgiFormSelectSingle("CR", crstr, sizeof(crstr)/sizeof(crstr[0]), &i, 0);
	wknet_cfg[id].CR = i;
}

void getlf() {
	
	int i;
	cgiFormSelectSingle("LF", crstr, sizeof(crstr)/sizeof(crstr[0]), &i, 0);
	wknet_cfg[id].LF = i;
}


void getmax_num() {
	
	int i;
	cgiFormSelectSingle("max_num", max_numtr, sizeof(max_numtr)/sizeof(max_numtr[0]), &i, 0);
	wknet_cfg[id].max_num= i;
}

void getatt() {
	
	int i;
	cgiFormSelectSingle("attestation", att_str, sizeof(att_str)/sizeof(att_str[0]), &i, 0);
	wknet_cfg[id].attestation = i;
}

void getreg() {
	
	int i;
	cgiFormSelectSingle("reg", str, sizeof(str)/sizeof(str[0]), &i, 0);
	wknet_cfg[id].reg = i;
}

void getsrv() {
	
	int i;
	cgiFormSelectSingle("srv_1st", str, sizeof(str)/sizeof(str[0]), &i, 0);
	wknet_cfg[id].srv_1st = i;
}

void getlport() {
	
	int i;
	wknet_cfg[id].lport= getformstrval("lport");
}

void getisnull() {
	
	int i;
	cgiFormSelectSingle("is_null", str, sizeof(str)/sizeof(str[0]), &i, 0);
	wknet_cfg[id].is_null = i;
}


void getprotocol()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "protocol_str_%d", j+1);
		cgiFormSelectSingle(lsbuf, protocol_str, sizeof(protocol_str)/sizeof(protocol_str[0]), &i, 0);
		wknet_cfg[id].session[j].protocol = i;
	}

}

void getip()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "ip_%d", j+1);
		getformstrchar(lsbuf, ipaddr, sizeof(ipaddr));
		wknet_cfg[id].session[j].ip = sys_str2ip(ipaddr);
	}

}

void getport()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "port_%d", j+1);
		wknet_cfg[id].session[j].port = getformstrval(lsbuf);
	}

}

void getconn()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "conn_%d", j+1);
		cgiFormSelectSingle(lsbuf, conn_str, sizeof(conn_str)/sizeof(conn_str[0]), &i, 0);
		wknet_cfg[id].session[j].conn = i;
	}

}

void getdisconn()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "dis_conn_%d", j+1);
		cgiFormSelectSingle(lsbuf, dis_conn_str, sizeof(dis_conn_str)/sizeof(dis_conn_str[0]), &i, 0);
		wknet_cfg[id].session[j].dis_conn = i;
	}

}

void gettime_out()
{
	int i,j;
	char lsbuf[32]="";
	for(j=0; j<MAX_SESSION; j++)
	{
		memset(lsbuf, 0, sizeof(lsbuf));
		sprintf(lsbuf, "time_out_%d", j+1);
		wknet_cfg[id].session[j].time_out = getformstrval(lsbuf);
	}

}


void print_css(void)
{
	fprintf(cgiOut, "<style type=\"text/css\">\n");
	fprintf(cgiOut, "<!--\n");
	fprintf(cgiOut, "body {\n");
	fprintf(cgiOut, "font-family: Arial, Helvetica, sans-serif;\n");
	fprintf(cgiOut, "font-size:12px;\n");
	fprintf(cgiOut, "color:#666666;\n");
	fprintf(cgiOut, "background:#fff;\n");
	fprintf(cgiOut, "text-align:center;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "* {\n");
	fprintf(cgiOut, "margin:0;\n");
	fprintf(cgiOut, "padding:0;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "a {\n");
	fprintf(cgiOut, "color:#1E7ACE;\n");
	fprintf(cgiOut, "text-decoration:none; \n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "a:hover {\n");
	fprintf(cgiOut, "color:#000;\n");
	fprintf(cgiOut, "text-decoration:underline;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "h3 {\n");
	fprintf(cgiOut, "font-size:14px;\n");
	fprintf(cgiOut, "font-weight:bold;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "pre,p {\n");
	fprintf(cgiOut, "color:#1E7ACE;\n");
	fprintf(cgiOut, "margin:4px;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "input, select,textarea {\n");
	fprintf(cgiOut, "padding:1px;\n");
	fprintf(cgiOut, "margin:2px;\n");
	fprintf(cgiOut, "font-size:11px;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, ".buttom{\n");
	fprintf(cgiOut, "padding:1px 10px;\n");
	fprintf(cgiOut, "font-size:12px;\n");
	fprintf(cgiOut, "border:1px #1E7ACE solid;\n");
	fprintf(cgiOut, "background:#c8d7e3;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "#formwrapper {\n");
	fprintf(cgiOut, "width:800px;\n");
	fprintf(cgiOut, "margin:15px auto;\n");
	fprintf(cgiOut, "padding:20px;\n");
	fprintf(cgiOut, "text-align:left;\n");
	fprintf(cgiOut, "border:1px solid #A4CDF2;\n");
	fprintf(cgiOut, "background:#c8d7e3;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "fieldset {\n");
	fprintf(cgiOut, "padding:10px;\n");
	fprintf(cgiOut, "margin-top:5px;\n");
	fprintf(cgiOut, "border:1px solid #A4CDF2;\n");
	fprintf(cgiOut, "background:#fff;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "fieldset legend {\n");
	fprintf(cgiOut, "color:#1E7ACE;\n");
	fprintf(cgiOut, "font-weight:bold;\n");
	fprintf(cgiOut, "padding:3px 20px 3px 20px;\n");
	fprintf(cgiOut, "border:1px solid #A4CDF2; \n");
	fprintf(cgiOut, "background:#fff;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "fieldset label {\n");
	fprintf(cgiOut, "float:left;\n");
	fprintf(cgiOut, "width:120px;\n");
	fprintf(cgiOut, "text-align:right;\n");
	fprintf(cgiOut, "padding:4px;\n");
	fprintf(cgiOut, "margin:1px;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, "fieldset div {\n");
	fprintf(cgiOut, "clear:left;\n");
	fprintf(cgiOut, "margin-bottom:2px;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, ".enter{ text-align:center;}\n");
	fprintf(cgiOut, ".clear {\n");
	fprintf(cgiOut, "clear:both;\n");
	fprintf(cgiOut, "}\n");
	fprintf(cgiOut, ".STYLE1 {font-size: 12%%}\n");
	fprintf(cgiOut, "-->\n");
	fprintf(cgiOut, "</style>\n");


}

