#
#  "$Id$"
#
#  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
#  All Rights Reserved.
#
#	Description:
#	Revisions:		Year-Month-Day  SVN-Author  Modification
#

CROSS = arm-uclibc-linux-
CC = $(CROSS)gcc
CPP = g++

EXEC_DIR = 
EXEC = app
DEBUG = -DDEBUG

OBJ = ./obj/
SRC = ./src/
INC = ./inc/
LIB = ./lib/
#XMLINC = /usr/local/libxml2/include/libxml2
#XMLINC = /home/xul/test/opt/hisilibxml2/include/libxml2

OBJS	=	$(OBJ)main.o		\
			$(OBJ)print.o		\
			$(OBJ)console.o		\
			$(OBJ)system.o		\
			$(OBJ)config.o		\
			$(OBJ)trd.o			\
			$(OBJ)xml.o			\
			$(OBJ)log.o			\
			$(OBJ)timer.o		\
			$(OBJ)file.o		\
			$(OBJ)iccard.o		\
			$(OBJ)dsp.o			\
			$(OBJ)database.o	\
			$(OBJ)dblog.o		\
			$(OBJ)net.o			\
			$(OBJ)indoor.o		\
			$(OBJ)outdoor.o		\


$(OBJ)%.o: $(SRC)%.c $(INC)*.h
	$(CC) -Wall -I$(INC) $(DEBUG) -g  -O0 -c -o  $(OBJ)$*.o $< 
default: $(OBJS) #$(INC)*.h
ifeq ($(CROSS), arm-uclibc-linux-)
#	$(CC) -O0 -o $(EXEC_DIR)$(EXEC) $(OBJS) -L$(LIB) -lxml2 -liconv -lpthread -lcrypt -lm 
	$(CC) -O0 -o $(EXEC_DIR)$(EXEC) $(OBJS) $(LIB)libxml2.a $(LIB)libiconv.a $(LIB)libsqlite3.a $(LIB)libhienc.a $(LIB)libhiapi.a -lpthread -lcrypt -lm -static
else
	$(CC) -O0 -o $(EXEC_DIR)$(EXEC) $(OBJS) -lpthread -lcrypt -liconv -lxml2 -lm $(LIB)libsqlite3_pc.a
endif

clean:
	rm -f $(EXEC) $(OBJ)*.o

