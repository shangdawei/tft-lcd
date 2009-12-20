CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
LIBS=-L./ -lcgic 
INC=../lib/


#${LIBS}
#mingw32 and cygwin users: replace .cgi with .exe

.c.o:
	$(CC) -I$(INC) -c -o $*.o $<

default: hostcfg.o 
	$(CC) hostcfg.o -o hostcfg.cgi ${LIBS} -I$(INC) $(INC)libucli.a
	cp hostcfg.cgi /var/www/cgi-bin/
#	sudo cp hostcfg.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi

