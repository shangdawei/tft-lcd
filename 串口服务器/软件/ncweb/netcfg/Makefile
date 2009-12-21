include ../config.mk

CFLAGS=-g -Wall
CC=$(CROSS)gcc

#mingw32 and cygwin users: replace .cgi with .exe
.c.o:
	$(CC) -I$(INC_DIR) -c -o $*.o $<
default: netcfg.o
	$(CC) netcfg.o -o netcfg.cgi $(LIB_DIR)libcgic.a $(LIB_DIR)libucli.a
	cp netcfg.cgi /var/www/cgi-bin/
#	sudo cp hostcfg.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi *.o

