include ../config.mk

CFLAGS=-g -Wall
CC=$(CROSS)gcc


#mingw32 and cygwin users: replace .cgi with .exe
.c.o:
	$(CC) -I$(INC_DIR) -c -o $*.o $<

default: mainframe.o
	$(CC) mainframe.o -o mainframe.cgi  $(LIB_DIR)libcgic.a $(LIB_DIR)libucli.a -lcrypt
	cp mainframe.cgi /var/www/cgi-bin/
#	sudo cp mainframe.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi *.o

