


CROSS = 
CC = gcc
LD = ld
LDFLAGS = -r -o
INC = ./
LIBFILE = libucli.a

OBJS = ./lucli.o

.c.o:
	$(CROSS)$(CC) -I$(INC)  -c -o $*.o $<

default: $(OBJS)
	$(CROSS)$(LD) $(LDFLAGS) $(LIBFILE) $(OBJS)

clean:
	rm -rf *.o *.a

