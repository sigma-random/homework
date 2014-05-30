###########################################
#Makefile for simple programs
###########################################
INC     = 
LIB     = -lpthread 

CC      = g++
CC_FLAG = -Wall -g

BIN     = TestPool
OBJ     = TestPool.o  ThreadPoolManage.o ThreadPool.o Thread.o WorkerThread.o 

$(BIN):$(OBJ)
	$(CC) $(INC) $(LIB) $(OBJ)    -o $@ 
	
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -g -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(BIN)
