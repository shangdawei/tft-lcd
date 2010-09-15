
#CROSS = powerpc-e300c3-linux-gnu-
CC = gcc
CPP = g++

EXEC_DIR = 
EXEC = app
DEBUG = -DDEBUG

OBJ = ./obj/
SRC = ./src/
INC = ./inc/
XMLINC = /usr/local/libxml2/include/libxml2

OBJS	=	$(OBJ)main.o		\
			$(OBJ)print.o		\
			$(OBJ)console.o		\
			$(OBJ)system.o		\
			$(OBJ)config.o		\
			$(OBJ)trd.o			\
			$(OBJ)xml.o			\


$(OBJ)%.o: $(SRC)%.c $(INC)*.h
	$(CC) -I$(INC) -I$(XMLINC) $(DEBUG) -g  -O0 -c -o  $(OBJ)$*.o $<
default: $(OBJS) #$(INC)*.h
	$(CC) -O0 -o $(EXEC_DIR)$(EXEC) $(OBJS) -lpthread -lcrypt -liconv -lxml2 -lm

clean:
	rm -f $(EXEC) $(OBJ)*.o

