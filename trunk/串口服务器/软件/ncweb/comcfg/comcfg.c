#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lucli.h"

 char* bpsstr[] = 
 {
	"---------------",
	"1200",
	"2400",
	"4800",
	"9600",
	"19200",
	"115200",
 };
 char* dbitstr[] = 
 {
	"---------------",
	"5",
	"6",
	"7",
	"8",
 }; 
 char* chkstr[] = 
 {
	"---------------",
	"偶校验",
	"奇校验",
	"无",
 };
 char* sbitstr[] = 
 {
	"---------------",
	"1",
	"1.5",
	"2",
 }; 

char* bctrlstr[] = 
 {
	"---------------",
	"X on/X off",
	"硬件",
	"无",
 };
char* str[] = 
 {
	"---------------",
	"no",
	"yes",
 };

char* typestr[] = 
 {
	"---------------",
	"RS232",
	"RS485",
	"RS422",
 };


void getbps();
void getdbit();
void getchk();
void getsbit();
void getbctrl();
void getdtr();
void getrts();
void getalive();
int getallYN();
void gettype();

int getmsg(CONFIG_COM* p);
int setmsg(CONFIG_COM* p);



void ShowForm();
void print_css(void);

char ipaddr[16]="";
char netmask[16]="";
char gateway[16]="";

static int id=0;
CONFIG_COM com_cfg[MAX_NC_PORT];

int cgiMain() 
{
	id = atoi(cgiQueryString);
	if(id > 0)
		id --;
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html;charset=gb2312");
	/* Top of the page */
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>NC600 Web Server</TITLE>\n");
	print_css();
	fprintf(cgiOut,	"</HEAD><BODY>\n");
	getmsg(com_cfg);
//	printf("%d\n",getmsg(com_cfg));
//	printf("bps = %d\n", com_cfg[id].bps);
	if(cgiFormSubmitClicked("host") == cgiFormSuccess)
	{
		getbps();
		getdbit();
		getchk();
		getsbit();
		getbctrl();
		getdtr();
		getrts();
		gettype();
		getalive();
		getallYN();
	//	printf("bps = %d\n", com_cfg[id].bps);
		setmsg(com_cfg);
		//printf("com_cfg.all %d\n", com_cfg.all);
	//	getpar("netmask", netmask);
	//	getpar("gateway", gateway);
	//	fprintf(cgiOut, "<H3>保存成功！</H3>\n");

	}
	ShowForm();
	fprintf(cgiOut, "</BODY></HTML>\n");
	
	return 0;
}


int getmsg(CONFIG_COM* p)
{
	int fd;
	
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}
	if(!get_com(fd, p))
	{
		close(fd);
		return 3;
	}

	close(fd);
	return 1;
}

int setmsg(CONFIG_COM* p)
{
	int fd;
	int i;
	CONFIG_COM tmp;
	fd = cli_conn(UCLI_PATH);
	if(fd < 0)
	{
		return 2;
	}

	if(p[id].all == 1)
	{
		memcpy(&tmp, &p[id], sizeof(CONFIG_COM));
		for(i = 0; i < MAX_NC_PORT; i++)
		{
			memcpy(&p[i], &tmp, sizeof(CONFIG_COM));
		}
	}
	
	if(!set_com(fd, p))
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
 fprintf(cgiOut, "	<h3>端口 %d 配置</h3>\n", id + 1);
 fprintf(cgiOut, "	<form action=\"\n");
 fprintf(cgiOut, "\" method=\"post\" name=\"com\" id=\"com\">\n");
 fprintf(cgiOut, "	  <fieldset>\n");
 fprintf(cgiOut, "	  <div>\n");
 fprintf(cgiOut, "		<label for=\"Name\">波特率</label>\n");
 fprintf(cgiOut, "		<select name=\"bps\" width=\"100\">\n");
 //fprintf(cgiOut, "	   <OPTION value=dW>---------------</OPTION>\n");
 for(i = 0; i < sizeof(bpsstr) / sizeof(bpsstr[0]); i++)
 {
	if(com_cfg[id].bps == atoi(bpsstr[i]))
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", bpsstr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", bpsstr[i]);
 }
// fprintf(cgiOut, "<option selected=\"selected\">%d</option>\n", com_cfg.bps);
// fprintf(cgiOut, "<option>1200</option>\n");
// fprintf(cgiOut, "<option>2400</option>\n");
// fprintf(cgiOut, "<option>4800</option>\n");
// fprintf(cgiOut, "<option>9600</option>\n");
// fprintf(cgiOut, "<option>115200</option>\n");
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	  <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">数据位</label>\n");
 fprintf(cgiOut, "		<select  name=\"dbit\"  width=\"100\">\n");
// fprintf(cgiOut, "	   <OPTION value=dW>---------------</OPTION>\n");

 for(i = 0; i < sizeof(dbitstr) / sizeof(dbitstr[0]); i++)
 {
	if(com_cfg[id].dbit == atoi(dbitstr[i]))
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", dbitstr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", dbitstr[i]);
 }
 /*
 fprintf(cgiOut, "<option selected=\"selected\">%d</option>\n", com_cfg.dbit);
     fprintf(cgiOut, "<option>5</option>\n");
    fprintf(cgiOut, "<option>6</option>\n");
    fprintf(cgiOut, "<option>7</option>\n");
    fprintf(cgiOut, "<option>8</option>\n");
    */
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	 <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">校验方式</label>\n");
 fprintf(cgiOut, "		<select  name=\"check\"  width=\"100\">\n");

 for(i = 0; i < sizeof(chkstr) / sizeof(chkstr[0]); i++)
 {
	if(com_cfg[id].chk == i)
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", chkstr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", chkstr[i]);
 }
 /*
 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", chkstr[com_cfg.chk]);
 fprintf(cgiOut, "<option>偶校验</option>\n");
 fprintf(cgiOut, "<option>奇校验</option>\n");
 fprintf(cgiOut, "<option>无</option>\n");
*/
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	 <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">停止位</label>\n");
 fprintf(cgiOut, "		<select name=\"sbit\"  width=\"100\">\n");
// fprintf(cgiOut, "	   <OPTION value=dW>---------------</OPTION>\n");
 for(i = 0; i < sizeof(sbitstr) / sizeof(sbitstr[0]); i++)
 {
	if(com_cfg[id].sbit == atof(sbitstr[i]))
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", sbitstr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", sbitstr[i]);
 }
 /*
 fprintf(cgiOut, "<option selected=\"selected\">%d</option>\n", com_cfg.sbit);
     fprintf(cgiOut, "<option>1</option>\n");
    fprintf(cgiOut, "<option>1.5</option>\n");
    fprintf(cgiOut, "<option>2</option>\n");
    */
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	 <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">数据流控制</label>\n");
 fprintf(cgiOut, "		<select  name=\"bctrl\"  width=\"100\">\n");
 for(i = 0; i < sizeof(bctrlstr) / sizeof(bctrlstr[0]); i++)
 {
	if(com_cfg[id].bctrl== i)
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", bctrlstr[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", bctrlstr[i]);
 }
 /*
 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", bctrlstr[com_cfg.bctrl]);
 fprintf(cgiOut, "<option>X on/X off</option>\n");
 fprintf(cgiOut, "<option>硬件</option>\n");
 fprintf(cgiOut, "<option>无</option>\n");
*/
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		 <br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	 <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">RTS&nbsp;&nbsp;ON</label>\n");
 fprintf(cgiOut, "		<select name=\"rts\" width=\"100\">\n");
 //fprintf(cgiOut, "	   <OPTION value=dW>---------------</OPTION>\n");
 for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
 {
	if(com_cfg[id].RTS == i)
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", str[i]);
 }
 /*
  fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[com_cfg.RTS]);
 fprintf(cgiOut, "		  <option>&nbsp;&nbsp;no</option>\n");
 fprintf(cgiOut, "		 <option>&nbsp;&nbsp;yes</option>\n");
 */
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	 <div>\n");


 fprintf(cgiOut, "		<label for=\"password\">DTR&nbsp;&nbsp;ON</label>\n");
 fprintf(cgiOut, "		<select  name=\"dtr\" width=\"100\">\n");
 //fprintf(cgiOut, "	   <OPTION value=dW>---------------</OPTION>\n");
 for(i = 0; i < sizeof(str) / sizeof(str[0]); i++)
 {
	if(com_cfg[id].DTR== i)
		fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[i]);
	else
		fprintf(cgiOut, "<option>%s</option>\n", str[i]);
 }
 /*
  fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[com_cfg.DTR]);
 fprintf(cgiOut, "		  <option>&nbsp;&nbsp;no</option>\n");
 fprintf(cgiOut, "		 <option>&nbsp;&nbsp;yes</option>\n");
 */
 fprintf(cgiOut, "	   </select>\n");
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");

 fprintf(cgiOut, "	  <div>\n");
  fprintf(cgiOut, " 	 <label for=\"password\">类型</label>\n");
  fprintf(cgiOut, " 	 <select name=\"type\" width=\"100\">\n");
  //fprintf(cgiOut, "		<OPTION value=dW>---------------</OPTION>\n");
  for(i = 0; i < sizeof(typestr) / sizeof(typestr[0]); i++)
  {
	 if(com_cfg[id].type == i)
		 fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", typestr[i]);
	 else
		 fprintf(cgiOut, "<option>%s</option>\n", typestr[i]);
  }
  /*
   fprintf(cgiOut, "<option selected=\"selected\">%s</option>\n", str[com_cfg.RTS]);
  fprintf(cgiOut, " 	   <option>&nbsp;&nbsp;no</option>\n");
  fprintf(cgiOut, " 	  <option>&nbsp;&nbsp;yes</option>\n");
  */
  fprintf(cgiOut, " 	</select>\n");
  fprintf(cgiOut, " 	 <br/>\n");
  fprintf(cgiOut, "    </div>\n");
  fprintf(cgiOut, "   <div>\n");



 
 fprintf(cgiOut, "	 <div>\n");
 fprintf(cgiOut, "		<label for=\"password\">保活时间</label>\n");
 fprintf(cgiOut, "		<input type=\"text\" name=\"alive\" size=\"9\" maxlength=\"9\" value=\"%d\"/>\n", com_cfg[id].alive);
 fprintf(cgiOut, "		<br/>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	  <div class=\"cookiechk\">\n");
 fprintf(cgiOut, "		<label>\n");
 fprintf(cgiOut, "	  \n");
 fprintf(cgiOut, "		</label>\n");
 fprintf(cgiOut, "		<input name=\"host\" type=\"submit\" class=\"buttom\" value=\"提交\" />\n");
 fprintf(cgiOut, "		 \n");
 fprintf(cgiOut, "		 <input type=\"checkbox\" name=\"AllYN\" id=\"CookieYN\" value=\"1\" />\n");
 fprintf(cgiOut, "		<a title=\"选择是否应用到所有串口\">设置应用到所有串口</a>\n");
 fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	  <div class=\"forgotpass\"></div>\n");
 fprintf(cgiOut, "	  </fieldset>\n");
 fprintf(cgiOut, "	</form>\n");
 fprintf(cgiOut, "	<br/>\n");
 fprintf(cgiOut, "	<h3>&nbsp;</h3>\n");
 fprintf(cgiOut, "</div>\n");

}


void getbps() {
	
	int i;
	cgiFormSelectSingle("bps", bpsstr, sizeof(bpsstr)/sizeof(bpsstr[0]), &i, 0);
	com_cfg[id].bps = atoi(bpsstr[i]);
}


void getdbit() {
	
	int i;
	cgiFormSelectSingle("dbit", dbitstr, sizeof(dbitstr)/sizeof(dbitstr[0]), &i, 0);
	com_cfg[id].dbit = atoi(dbitstr[i]);
}

void getchk() {
	
	int i;
	cgiFormSelectSingle("check", chkstr, sizeof(chkstr)/sizeof(chkstr[0]), &i, 0);
	com_cfg[id].chk= i;
}

void getsbit() {
	
	int i;
	cgiFormSelectSingle("sbit", sbitstr, sizeof(sbitstr)/sizeof(sbitstr[0]), &i, 0);
	com_cfg[id].sbit= atof(sbitstr[i]);
}

void getbctrl() {
	
	int i;
	cgiFormSelectSingle("bctrl", bctrlstr, sizeof(bctrlstr)/sizeof(bctrlstr[0]), &i, 0);
	com_cfg[id].bctrl = i;
}


void getdtr() {
	
	int i;
	cgiFormSelectSingle("dtr", str, sizeof(str)/sizeof(str[0]), &i, 0);
	com_cfg[id].DTR = i;
}

void getrts() {
	
	int i;
	cgiFormSelectSingle("rts", str, sizeof(str)/sizeof(str[0]), &i, 0);
	com_cfg[id].RTS = i;
}


void getalive()
{
	char name[10]="";
	cgiFormStringNoNewlines("alive", name, 10);
	com_cfg[id].alive = atoi(name);

}

void gettype() {
	
	int i;
	cgiFormSelectSingle("type", typestr, sizeof(typestr)/sizeof(typestr[0]), &i, 0);
	com_cfg[id].type = i;
}


int getallYN()
{
	if(cgiFormCheckboxSingle("AllYN") == cgiFormSuccess)
	{
		com_cfg[id].all = 1;
		return 1;
	}
	com_cfg[id].all = 0;
	return 0;
}

void Passwd(char *passwd) {

	cgiFormStringNoNewlines("passwd", passwd, 81);
	fprintf(cgiOut, "Passwd: ");
	cgiHtmlEscape(passwd);
	fprintf(cgiOut, "<BR>\n");
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


