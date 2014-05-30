###########################################
#Makefile for simple programs
###########################################
LIB     = -lpthread -lmysqlclient

CC      = g++
CC_FLAG = -g  #-Wall -g

BIN     = Server


Server_OBJ     =		\
	INIParser.o 		\
	Logger.o 			\
	Config.o 			\
	TcpSocket.o 		\
	TcpServer.o 		\
	RequestHandler.o 	\
	DBInstance.o 		\
	GetFileJob.o 		\
	QueryDBJob.o 		\
	XMLParser.o 		\
	main.o



ThreadPool_DIR =   ./ThreadPool/
ThreadPool_OBJ =							\
	$(ThreadPool_DIR)Thread.o 				\
	$(ThreadPool_DIR)WorkerThread.o 		\
	$(ThreadPool_DIR)ThreadPool.o 			\
	$(ThreadPool_DIR)ThreadPoolManage.o

	
$(BIN):$(Server_OBJ) $(ThreadPool_OBJ)
	$(CC) $(INC) $(LIB) $(Server_OBJ) $(ThreadPool_OBJ)  -o $@ 
	


.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC)  -c $(CC_FLAG) $*.cpp  -o  $*.o


.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(Server_OBJ) $(ThreadPool_OBJ) $(BIN)
