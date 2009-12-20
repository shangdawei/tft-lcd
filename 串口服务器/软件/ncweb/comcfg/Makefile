CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
LIBS=-L./ -lcgic 
INC=../lib/

#mingw32 and cygwin users: replace .cgi with .exe

.c.o:
	$(CC) -I$(INC) -c -o $*.o $<
default: comcfg.o libcgic.a 
	$(CC) comcfg.o -o comcfg.cgi ${LIBS} -I$(INC) $(INC)libucli.a
	cp comcfg.cgi /var/www/cgi-bin/
#	sudo cp hostcfg.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi *.o

