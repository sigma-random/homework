###########################################
#Makefile for simple programs
###########################################
LIB     =

CC      = g++
CC_FLAG = -g  #-Wall -g

BIN     = Client


Client_OBJ     =			\
	INIParser.o 			\
	Logger.o 				\
	Config.o 				\
	TcpClient.o 			\
	SearchWordService.o 	\
	RequestPaperService.o 	\
	main.o 


	
$(BIN):$(Client_OBJ) 
	$(CC) $(INC) $(LIB) $(Client_OBJ)  -o $@ 
	


.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC)  -c $(CC_FLAG) $*.cpp  -o  $*.o


.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(Client_OBJ)  $(BIN)
