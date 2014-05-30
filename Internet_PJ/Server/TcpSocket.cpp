#include "include/TcpSocket.h"


TcpSocket::TcpSocket(const char* ip,int port)
{
	assert(ip != NULL);
	m_IP = ip;
	assert(port > 0);
	m_Port = port;
	cout<<"\n[DEBUG] Set up Socket Server"<<endl;
}

TcpSocket::~TcpSocket()
{
	if(m_Sockfd)
	{
		close(m_Sockfd);
	}
	if(m_Sockaddr)
	{
		free(m_Sockaddr);
	}

}

void 
TcpSocket::startTCPServer()
{
	initSocket();
	buildSockAddr();
	bindListen();
}

void 
TcpSocket::startTCPClient()
{

}

void 
TcpSocket::initSocket()
{
		if(0 >= (m_Sockfd = socket(AF_INET,SOCK_STREAM,0)))
		{
			perror("init socket error!");
			exit(-1);
		}
}

void 
TcpSocket::setMaxConn(int maxconn)
{

	m_Maxconn = maxconn;
	cout<<"[DEBUG] Set MaxConn: "<<maxconn<<endl;
}

void 
TcpSocket::buildSockAddr()
{
	m_Sockaddr = (SOCKADDR_IN*)malloc(sizeof (SOCKADDR_IN));

	m_Sockaddr->sin_family = AF_INET;
	m_Sockaddr->sin_addr.s_addr =inet_addr(m_IP);
	m_Sockaddr->sin_port = htons(m_Port);
}


void 
TcpSocket::bindListen(int maxconn)
{
	socklen_t addrlen;
	int flag;
	assert(m_Sockfd > 0);
	assert(m_Sockaddr != NULL);
	assert(maxconn >= 0);
	m_Maxconn = maxconn;
	addrlen = sizeof(SOCKADDR);

	if( 0 != setsockopt(m_Sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, addrlen)) 
   	{ 
		perror("setsockopt SO_REUSEADDR"); 
		exit(-1); 
   	} 	
	
	if(0 != bind(m_Sockfd, (SOCKADDR*)m_Sockaddr, addrlen))
	{	
		perror("bind error!");
		exit(-1);
	}

	if(0 != listen(m_Sockfd,m_Maxconn))
	{	
		perror("listen error!");
		exit(-1);
	}
	cout<<"[DEBUG] Serve: "<<m_IP<<":"<<m_Port<<endl;
}

int 
TcpSocket::acceptClient()
{
	int connfd;
	SOCKADDR_IN cliAddr;
	socklen_t addrlen;

	addrlen = sizeof(cliAddr);
	if(0>=(connfd = accept(m_Sockfd, (SOCKADDR*)&cliAddr, &addrlen)))
	{
		perror("accept error");
		return -1;
	}
	
	cout<<"[DEBUG] accept a new client: "<<inet_ntoa(cliAddr.sin_addr)<<":"<<ntohs(cliAddr.sin_port)<<endl;
	
	m_Connfd = connfd;

	return connfd;

}

