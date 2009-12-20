CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
LIBS=-L./ -lcgic 


#mingw32 and cygwin users: replace .cgi with .exe

default: mainframe.o libcgic.a 
	$(CC) mainframe.o -o mainframe.cgi ${LIBS} 
	cp mainframe.cgi /var/www/cgi-bin/
#	sudo cp mainframe.cgi ~/nfshome/var/www/cgi-bin/
clean:
	rm -f *.cgi

