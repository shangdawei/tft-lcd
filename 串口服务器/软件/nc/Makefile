
CROSS = arm-linux-
GCC = $(CROSS)gcc
EXEC = app
INC	=./inc/
SRC = ./src/
OBJS	=	  main.o			\
		$(SRC)system/system.o	\
		$(SRC)log/sys_log.o		\
		$(SRC)file/file.o		\
		$(SRC)usocket/usocket.o	\
		$(SRC)net/net.o			\
		$(SRC)com/com.o			\
		$(SRC)config/conf.o		\
		$(SRC)thread/thread.o	\

.c.o:
	$(GCC) -I$(INC) -g -c -o $*.o $<

default:	$(OBJS)
	$(GCC) -O3 -o $(EXEC) $(OBJS) -lpthread
	sudo cp app /home/xul/nfshome/home/bin/
clean:
	rm -rf *.o
	rm -rf $(SRC)/*/*.o
