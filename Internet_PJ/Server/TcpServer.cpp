
#include "include/TcpServer.h"
#include "include/Logger.h"

TcpServer::~TcpServer()
{
	Logger("Call ~TcpServer()\n");
}

void 
TcpServer::startSearchServer(int maxconn)
{
	assert(m_IP != NULL);
	assert(m_Port > 0);
	assert(maxconn > 0);
	setMaxConn(maxconn);
	startTCPServer();
	assert(m_Sockfd > 0 );
	m_Listenfd = m_Sockfd;
	
}

bool 
TcpServer::register_handler(TcpSocketHandler* handler)
{
    assert(handler != NULL );
    m_Handler = handler;
    m_Handler->setServerfd(m_Listenfd);
    return true;
}

