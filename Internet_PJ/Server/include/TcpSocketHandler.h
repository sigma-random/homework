#ifndef __TCP_SOCKET__HANDLER__
#define __TCP_SOCKET__HANDLER__

//Interface
class TcpSocketHandler
{

public:
	TcpSocketHandler():m_Serverfd(0),m_Clientfd(0){};
	virtual ~TcpSocketHandler(){};
	virtual int handleClient(int serverfd)=0;
	virtual void request_handle(int clientfd )=0;
	void	setServerfd(int serverfd){m_Serverfd=serverfd;}
	int		getServerfd(){return m_Serverfd;}
private:
	int m_Serverfd;
	int m_Clientfd;
};

#endif