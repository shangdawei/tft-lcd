include ../config.mk

CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
#LIBS=-L./ -lcgic 

#mingw32 and cygwin users: replace .cgi with .exe

.c.o:
	$(CROSS)$(CC) -I$(INC_DIR) -c -o $*.o $<
default: user.o  
	$(CROSS)$(CC) user.o -o user.cgi $(LIB_DIR)libcgic.a $(LIB_DIR)libucli.a -lcrypt
#	cp user.cgi /var/www/cgi-bin/
#	sudo cp hostcfg.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi *.o

