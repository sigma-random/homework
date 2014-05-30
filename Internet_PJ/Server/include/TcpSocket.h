#ifndef __TCP_SOCKET__
#define __TCP_SOCKET__

#include "Common.h"


class TcpSocket
{

public:
	TcpSocket(const char* ip = "0.0.0.0",int port = 80);
	virtual ~TcpSocket();
	void initSocket();
	void buildSockAddr();
	void setMaxConn(int maxconn);
	void bindListen(int maxconn=100);
	void startTCPServer();			// to be tcp socket server
	void startTCPClient();			// to be tcp socket client
	int  acceptClient();			// to be tcp socket server
	void connectServer(){};			// to be tcp socket client


public:
	int m_Sockfd;		
	int m_Connfd;		
	const char *m_IP;
	int m_Port;
	SOCKADDR_IN *m_Sockaddr;
	int m_Maxconn;
	

};

#endif
