#ifndef __SEARCH_SERVER__
#define __SEARCH_SERVER__


#include "Common.h"
#include "TcpSocket.h"
#include "TcpSocketHandler.h"

class TcpServer:public TcpSocket
{

public:
	TcpServer(const char* ip,int port):TcpSocket(ip,port){};
	virtual ~TcpServer();
	void startSearchServer(int maxconn = 50);
	bool register_handler(TcpSocketHandler* handler);
	
private:
	int m_Listenfd;
	TcpSocketHandler* m_Handler;
};

#endif
