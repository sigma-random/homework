#include "include/TcpClient.h"

TcpClient::TcpClient(const char* remoteIP ,int remotePort,int timeout)
{
	m_RemoteIP = remoteIP;
	m_RemotePort = remotePort;
	initSocket();
	buildSockAddr();
	connectServer();
}

TcpClient::~TcpClient()
{
}

void TcpClient::initSocket()
{
	m_Connfd = socket(AF_INET,SOCK_STREAM,0);
	if(m_Connfd < 0) 
    {
       perror("socket");
       exit(-1);
    }

}


void TcpClient::buildSockAddr()
{
	m_ServerAddr = (SOCKADDR_IN*)malloc(sizeof(SOCKADDR_IN));
	assert(m_ServerAddr > 0);
	bzero(m_ServerAddr,sizeof(SOCKADDR_IN));
    m_ServerAddr->sin_family=AF_INET;
    m_ServerAddr->sin_port=htons(m_RemotePort);
    inet_pton(AF_INET,m_RemoteIP,&m_ServerAddr->sin_addr);
}

void TcpClient::connectServer()
{
    if(0 != connect(m_Connfd,(struct sockaddr *)m_ServerAddr,sizeof(SOCKADDR_IN)))
    {    
		perror("connect");
		exit(-1);
    }
    printf("[DEBUG] success to connect server %s:%d\n",m_RemoteIP,m_RemotePort);

}


char* TcpClient::queryDataOnce(const char* data)
{
	struct timeval timeout = {0,0};
	char   recvline[512] = {0};
	int    read_cnt = 0;
	int    total_cnt = 0;
	fd_set rset;
	string ret_data = "";

	write(m_Connfd,data,strlen(data));       

	FD_ZERO(&rset);

	for(;;)
	{
		FD_SET(m_Connfd,&rset);
		timeout.tv_sec  = 3;
		timeout.tv_usec = 0;	// 3 seconds        
		//select(m_Connfd +1,&rset,NULL,NULL,&timeout);  	
		select(m_Connfd +1,&rset,NULL,NULL,NULL);      
		if( FD_ISSET(m_Connfd,&rset))
		{	
			bzero(&recvline,strlen(recvline));
			if(0==(read_cnt=read(m_Connfd,recvline,sizeof(recvline))))
			{ 
				//printf("read finished!!!\n");
				break;
			}
			total_cnt += read_cnt;
			ret_data.append(recvline,read_cnt);
			//fputs(recvline,stdout); 
		}                          
	}
	//fputs(ret_data.c_str(),stdout); 
	//fputs(ret_data.data(),stdout); 
	return (char*)ret_data.c_str();
}

void TcpClient::interact()
{
	struct timeval timeout = {0,0};
    int cr = 0;
    char recv[512];
    FILE* fp = stdin;
    int maxfd;
    int nready;
    fd_set rset;
    char sendline[512],recvline[512];

	FD_ZERO(&rset);
    nready = 0;
    for(;;)
    {
        FD_SET(fileno(fp),&rset);
        FD_SET(m_Connfd,&rset);
        
        maxfd=std::max(fileno(fp),m_Connfd) +1;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;	// 3 seconds        
        nready = select(maxfd,&rset,NULL,NULL,&timeout);            //使用select使得客户端不需要注释在标准IO 或者 TCP read的其中一个上

        if( FD_ISSET(m_Connfd,&rset))
        {
            if(0>=(cr=read(m_Connfd,recvline,sizeof(recvline))))
            //那边发来需要有换行符表示串尾，不然这边要求接受的字节数还不到，或者还没到尾部，所以没有输出
            { 
                printf("read error\n");
                return;
            }
        	//printf("readed:%d,str:%d\n",cr,strlen(recvline));
        	//如果tcp缓冲区内有多于预定义取的字节，则会自动调用read再取，直到结尾或者遇到换行符
            fputs(recvline,stdout);                            //标准库函数fputs向标准IO上写一行
            bzero(&recvline,strlen(recvline));
        }
        if( FD_ISSET(fileno(fp),&rset))
        {
            if(fgets(sendline,sizeof(sendline),fp)==NULL)    //标准库函数fgets从标准IO上获取一行
            //fgets会加上换行符，如果有
                return;
            write(m_Connfd,sendline,strlen(sendline)-1);        
            //减一去除换行符
            bzero(&sendline,strlen(sendline));
        }
    }
    
}




//export Class Public API
//#######################################################################################

extern "C" {

TcpClient* TcpClient_Wrapper(const char* remoteIP ,int remotePort,int timeout=0)
{
	return new TcpClient(remoteIP, remotePort,timeout); 
}

char* TcpClient_queryDataOnce(TcpClient* client,const char* data)
{
	return client->queryDataOnce(data);
}

}

//#######################################################################################