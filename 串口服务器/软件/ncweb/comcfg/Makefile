include ../config.mk

CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
#LIBS=-L./ -lcgic 

#mingw32 and cygwin users: replace .cgi with .exe

.c.o:
	$(CROSS)$(CC) -I$(INC_DIR) -c -o $*.o $<
default: comcfg.o  
	$(CROSS)$(CC) comcfg.o -o comcfg.cgi $(LIB_DIR)libucli.a $(LIB_DIR)libcgic.a
	cp comcfg.cgi /var/www/cgi-bin/
#	sudo cp hostcfg.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi *.o

