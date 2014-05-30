
CC = g++

SERVER_BIN = fileserver
SERVER_SRC = Server.cpp FileServer.cpp
SERVER_TAR_OBJ = Server.o
SERVER_OBJ = $(SERVER_TAR_OBJ) FileServer.o
SERVER_HDR = FileServer.h common.h

CLIENT_BIN = fileclient
CLIENT_SRC = Client.cpp FileClient.cpp
CLIENT_TAR_OBJ = Client.o 
CLIENT_OBJ = $(CLIENT_TAR_OBJ)  FileClient.o
CLIENT_HDR = FileClient.h common.h

server : $(SERVER_OBJ)
	$(CC) -o $(SERVER_BIN) $(SERVER_OBJ)

$(SERVER_TAR_OBJ): $(SERVER_SRC) $(SERVER_HDR)

ServerFile.0: $(SERVER_HDR)



client : $(CLIENT_OBJ)
	$(CC) -o $(CLIENT_BIN) $(CLIENT_OBJ)

$(CLIENT_TAR_OBJ): $(CLIENT_SRC) $(CLIENT_HDR)
	$(CC) -c $(CLIENT_SRC)

FileClient.o:$(CLIENT_HDR)



help:
	@echo [1] make server 
	@echo [2] make client
	@echo [3] make clean

clean : 
	rm $(SERVER_BIN) $(SERVER_OBJ)
	rm $(CLIENT_BIN) $(CLIENT_OBJ)
