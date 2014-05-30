g++ -c -fPIC TcpClient.cpp -o TcpClient.o
g++ -shared -Wl,-soname,libTcpCLient.so TcpClient.o -o libTcpClient.so
