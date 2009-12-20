#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lucli.h"


char* modestr[] = 
 {
	"---------------",
	"auto",
 };

char* str[] = 
 {
	"---------------",
	"no",
	"yes",
 };

void getpar(char *src,char *name); 
void ShowForm();
int get_msg(CONFIG_NET *p);
int set_msg(CONFIG_NET *p);
void getformstrchar(char *name, char *buf, int size);
int getformstrval(char *name);

void getmode();
void getdhcp();



CONFIG_NET net_cfg;



char ipaddr[16]="";
char netmask[16]="";
char gateway[16]="";
char dns[2][16];


void print_css(void);


int cgiMain() 
{
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html;charset=gb2312");
	/* Top of the page */
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>NC600 Web Server</TITLE>\n");
	print_css();
	fprintf(cgiOut,	"</HEAD><BODY>\n");
	get_msg(&net_cfg);
 	//fprintf(cgiOut, "ret = \%d\n", ret);
	sys_ip2str(net_cfg.dev_ip, ipaddr);
 	sys_ip2str(net_cfg.dev_nm, netmask);
 	sys_ip2str(net_cfg.dev_gw, gateway);
 	sys_ip2str(net_cfg.dns[0], dns[0]);
  	sys_ip2str(net_cfg.dns[1], dns[1]);
	if(cgiFormSubmitClicked("host") == cgiFormSuccess)
	{
		getformstrchar("host_name", net_cfg.host_name, sizeof(net_cfg.host_name));
		getformstrchar("host_pos", net_cfg.host_pos, sizeof(net_cfg.host_pos));
		getformstrchar("dev_ip", ipaddr, sizeof(ipaddr));
		getformstrchar("dev_nm", netmask, sizeof(netmask));
		getmode();
		getdhcp();
		getformstrchar("dev_gw", gateway, sizeof(gateway));
		getformstrchar("dns1", dns[0], sizeof(dns[0]));
		getformstrchar("dns2", dns[1], sizeof(dns[1]));
		net_cfg.time_out = getformstrval("time_out");
		net_cfg.dhcp_id = getformstrval("dhcp_id");

		net_cfg.dev_ip = sys_str2ip(ipaddr);
		net_cfg.dev_nm = sys_str2ip(netmask);
		net_cfg.dev_gw = sys_str2ip(gateway);
		net_cfg.dns[0] = sys_str2ip(dns[0]);
		net_cfg.dns[1] = sys_str2ip(dns[1]);
		set_msg(&net_cfg);
	}
	ShowForm();
	fprintf(cgiOut, "</BODY></HTML>\n");
	
	return 0;
}

int set_msg(CONFIG_NET *p)
{
	int fd;
	CONFIG_COM tmp;
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}

	if(!set_net(fd, p))
	{
		close(fd);
		return 3;
	}

	close(fd);
	return 1;

}


int get_msg(CONFIG_NET *p)
{
	int fd;
	
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}
	if(!get_net(fd, p))
	{
		close(fd);
		return 3;
	}

	close(fd);
	return 1;

}

void ShowForm()
{

  int i ;



 
  fprintf(cgiOut, "<div id=\"formwrapper\">\n");
  fprintf(cgiOut, "  <h3>服务器配置</h3>\n");
  fprintf(cgiOut, "  <form action=\"\n");
  fprintf(cgiOut, "\" method=\"post\" name=\"com\" id=\"com\">\n");
  fprintf(cgiOut, "    <fieldset>\n");

  
  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">服务器名</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"host_name\" size=\"32\" maxlength=\"32\" value=\"%s\"/>\n", net_cfg.host_name);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">服务器位置</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"host_pos\" size=\"32\" maxlength=\"32\" value=\"%s\"/>\n", net_cfg.host_pos);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">以太网IP地址</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dev_ip\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", ipaddr);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">以太网子网掩码</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dev_nm\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", netmask);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, " 	 <label for=\"password\">以太网工作模式</label>\n");
  fprintf(cgiOut, " 	 <select  name=\"mode\"  width=\"100\">\n");
  for(i = 0; i < sizeof(modestr) / sizeof(modestr[0]); i++)
  {
	 if(net_cfg.mode == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", modestr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", modestr[i]);
  }
  fprintf(cgiOut, " 	</select>\n");
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">启用DHCP</label>\n");
  fprintf(cgiOut, " 	 <select  name=\"dev_dhcp\"  width=\"100\">\n");
  for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
  {
	 if(net_cfg.dev_dhcp == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", str[i]);
  }
  fprintf(cgiOut, " 	</select>\n");
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");


  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">DHCP CLIENT ID</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dhcp_id\" size=\"15\" maxlength=\"15\" value=\"%d\"/>\n", net_cfg.dhcp_id);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">默认网关</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dev_gw\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", gateway);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">主DNS服务器</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dns1\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", dns[0]);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");

  fprintf(cgiOut, "   <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">次DNS服务器</label>\n");
  fprintf(cgiOut, " 	 <input type=\"text\" name=\"dns2\" size=\"15\" maxlength=\"15\" value=\"%s\"/>\n", dns[1]);
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "    <div>\n");


fprintf(cgiOut, "	<div>\n");
fprintf(cgiOut, "	   <label for=\"password\">CONSOLE超时时间</label>\n");
fprintf(cgiOut, "	   <input type=\"text\" name=\"time_out\" size=\"15\" maxlength=\"15\" value=\"%d\"/>\n", net_cfg.time_out);
fprintf(cgiOut, "	   <br/>\n");
fprintf(cgiOut, "	 </div>\n");
fprintf(cgiOut, "	 <div>\n");

fprintf(cgiOut, "    <div class=\"cookiechk\">\n");
fprintf(cgiOut, "      <label>\n");
fprintf(cgiOut, "      \n");
fprintf(cgiOut, "      </label>\n");
fprintf(cgiOut, "      <input name=\"host\" type=\"submit\" class=\"buttom\" value=\"提交\" />\n");
fprintf(cgiOut, "    </div>\n");


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
	net_cfg.mode = i;
}

void getdhcp() {
	
	int i;
	cgiFormSelectSingle("dev_dhcp", str, sizeof(str)/sizeof(str[0]), &i, 0);
	net_cfg.dev_dhcp= i;
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
	fprintf(cgiOut, "width:450px;\n");
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
	fprintf(cgiOut, "-->\n");
	fprintf(cgiOut, "</style>\n");


}

