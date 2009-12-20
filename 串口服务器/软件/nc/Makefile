
GCC = gcc
EXEC = app
INC	=./inc/
SRC = ./src/
OBJS	=	  main.o			\
		$(SRC)system/system.o	\
		$(SRC)log/sys_log.o		\
		$(SRC)file/file.o		\
		$(SRC)usocket/usocket.o	\
		$(SRC)net/net.o			\
		$(SRC)config/conf.o

.c.o:
	$(GCC) -I$(INC) -g -c -o $*.o $<

default:	$(OBJS)
	$(GCC) -O3 -o $(EXEC) $(OBJS) -lpthread
clean:
	rm -rf *.o
	rm -rf $(SRC)/*/*.o
