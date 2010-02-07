#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 




static char name[16]="", passwd[16]="";



void ShowForm();
void print_css(void);
void print_backgrounf(void);
int get_user(char* name, char* passwd);
void getformstrchar(char *name, char *buf, int size);
int set_user(char* name, char* passwd);


int cgiMain() 
{


	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html;charset=gb2312");
	/* Top of the page */
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>NC600 Web Server</TITLE>\n");
	print_css();
	print_backgrounf();
	fprintf(cgiOut,	"</HEAD><BODY>\n");
	get_user(name, passwd);

	if(cgiFormSubmitClicked("host") == cgiFormSuccess)
	{
		char o_name[16]="", o_passwd[16]="", n_name[16]="", n_passwd[16]="", nck_passwd[16]="";
		getformstrchar("name", o_name, 16);
		getformstrchar("passwd", o_passwd, 16);
		getformstrchar("n_name", n_name, 16);
		getformstrchar("n_passwd", n_passwd, 16);
		getformstrchar("nck_passwd", nck_passwd, 16);
		printf("[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n", o_name, o_passwd, n_name, n_passwd, nck_passwd);		
		printf("[%s]\t[%s]\t[%s]\n", name, passwd, (char*)crypt(o_passwd, "NC600"));
		if((strcmp(name, o_name) == 0) && (strcmp(passwd, (char*)crypt(o_passwd, "NC600")) == 0) && (strcmp(n_passwd, nck_passwd) == 0))
		{
				set_user(n_name, n_passwd);		
				get_user(name, passwd);
				fprintf(cgiOut, "<script type=\"text/javascript\">\n");
				fprintf(cgiOut, "window.alert(\"口令修改成功！\");\n");
				fprintf(cgiOut, "</script>\n");
		}
		else
		{
				fprintf(cgiOut, "<script type=\"text/javascript\">\n");
				fprintf(cgiOut, "window.alert(\"口令修改失败！\");\n");
				fprintf(cgiOut, "</script>\n");
		}

	}
	ShowForm();
	fprintf(cgiOut, "</BODY></HTML>\n");
	
	return 0;
}

int set_user(char* name, char* passwd)
{
		int fd = 0;
		char lsbuf[64]="";
		fd = open("/etc/nc/shadow", O_WRONLY | O_TRUNC);
		if(fd <= 0) return 0;
			
		sprintf(lsbuf, "%s:%s", name, crypt(passwd, "NC600"));
		if(write(fd, lsbuf, strlen(lsbuf)) > 0)
		{
			close(fd);
			return 1;
		}
		else
		{
			close(fd);
			return 1;			
		}
}

int get_user(char* name, char* passwd)
{
	int fd = 0;	
	char lsbuf[64]="";

	fd = open("/etc/nc/shadow", O_RDONLY);
	if(fd <= 0) return 0;
	read(fd, lsbuf, 64);


	strcpy(name, strtok(lsbuf, ":"));
	strcpy(passwd, strtok(NULL, ":"));

	return 1;
}

void getformstrchar(char *name, char *buf, int size)
{
	cgiFormStringNoNewlines(name, buf, size);
}


void ShowForm()
{
 int i ;

 fprintf(cgiOut, "<div id=\"formwrapper\">\n");

 fprintf(cgiOut, "	<form action=\"");
 fprintf(cgiOut, "\" method=\"post\" name=\"com\" id=\"com\">\n");
 fprintf(cgiOut, "	  <fieldset>\n");
 fprintf(cgiOut, "   <div>\n");
fprintf(cgiOut, "	  <label for=\"password\">原用户名</label>\n");
fprintf(cgiOut, "	  <input type=\"text\" name=\"name\" value=\"%s\" size=\"16\" maxlength=\"32\" />\n", name);
fprintf(cgiOut, "	  <br/>\n");
fprintf(cgiOut, "</div>\n");
fprintf(cgiOut, "	<div>\n");
fprintf(cgiOut, "   <div>\n");
fprintf(cgiOut, "	  <label for=\"password\">原密码</label>\n");
fprintf(cgiOut, "	  <input type=\"password\" name=\"passwd\" size=\"16\" maxlength=\"32\" />\n");
fprintf(cgiOut, "	  <br/>\n");
fprintf(cgiOut, "</div>\n");
fprintf(cgiOut, "   <div>\n");
fprintf(cgiOut, "	  <label for=\"password\">新用户名</label>\n");
fprintf(cgiOut, "	  <input type=\"text\" name=\"n_name\" size=\"16\" maxlength=\"32\" />\n");
fprintf(cgiOut, "	  <br/>\n");
fprintf(cgiOut, "</div>	\n");
fprintf(cgiOut, "   <div>\n");
fprintf(cgiOut, "	  <label for=\"password\">新密码</label>\n");
fprintf(cgiOut, "	  <input type=\"password\" name=\"n_passwd\" size=\"16\" maxlength=\"32\" />\n");
fprintf(cgiOut, "	  <br/>\n");
fprintf(cgiOut, "</div>	\n");
fprintf(cgiOut, "   <div>\n");
fprintf(cgiOut, "	  <label for=\"password\">确认新密码</label>\n");
fprintf(cgiOut, "	  <input type=\"password\" name=\"nck_passwd\" size=\"16\" maxlength=\"32\" />\n");
fprintf(cgiOut, "	  <br/>\n");
fprintf(cgiOut, "</div>	<div>\n");
fprintf(cgiOut, "	  <div class=\"cookiechk\">\n");
fprintf(cgiOut, "		<label>\n");
fprintf(cgiOut, "		\n");
fprintf(cgiOut, "		</label>\n");
fprintf(cgiOut, "		<input name=\"host\" type=\"submit\" class=\"buttom\" value=\"提交\" />		\n");
fprintf(cgiOut, "		<input name=\"host1\" type=\"reset\" class=\"buttom\" value=\"清空\" />\n");
fprintf(cgiOut, "	  </div>\n");
 fprintf(cgiOut, "	  <div class=\"forgotpass\"></div>\n");
 fprintf(cgiOut, "	  </fieldset>\n");
 fprintf(cgiOut, "	</form>\n");
 fprintf(cgiOut, "	<br/>\n");
 fprintf(cgiOut, "	<h3>&nbsp;</h3>\n");
 fprintf(cgiOut, "</div>\n");

}


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


