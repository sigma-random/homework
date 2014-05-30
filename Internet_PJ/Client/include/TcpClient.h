#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

#include "Common.h"


class TcpClient
{

public:
	TcpClient(const char* remoteIP = "0.0.0.0",int remotePort = 80,int timeout=0);
	virtual ~TcpClient();
	void initSocket();
	void buildSockAddr();
	void connectServer();
	char* queryDataOnce(const char* data);
	void interact();


public:	
	int m_Connfd;		
	const char *m_RemoteIP;
	int m_RemotePort;
	SOCKADDR_IN *m_ServerAddr;
};

#endif
