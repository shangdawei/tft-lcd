include ../config.mk


CC = gcc
LD = ld
LDFLAGS = -r -o
LIBFILE = libucli.a

OBJS = ./lucli.o

.c.o:
	$(CROSS)$(CC) -I$(INC_DIR)  -c -o $*.o $<

default: $(OBJS)
	$(CROSS)$(LD) $(LDFLAGS) $(LIBFILE) $(OBJS)
	cp $(LIBFILE) $(LIB_DIR)
clean:
	rm -rf *.o *.a

