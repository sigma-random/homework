#include "include/RequestHandler.h"


Job* RequestHandler::m_QueryDBJob     = NULL;
Job* RequestHandler::m_GetFileJob   = NULL;
ThreadPoolManage* RequestHandler::m_TPM = NULL;


RequestHandler::RequestHandler (){ }

RequestHandler::~RequestHandler(){ }


void 
RequestHandler::BindThreadPool(ThreadPoolManage* TPM)
{	
	assert(TPM != NULL );
	m_TPM = TPM;
}

void 
RequestHandler::BindDBServiceJob(Job* clientJob)
{	
	assert(m_TPM != NULL );
	assert(clientJob != NULL );
	m_QueryDBJob = clientJob;

}

void 
RequestHandler::BindFileServiceJob(Job* clientJob)
{	
	assert(m_TPM != NULL );
	assert(clientJob != NULL );
	m_GetFileJob = clientJob;

}

//static methos
void  
RequestHandler::RunJob(int clientfd, Job* job, char *job_param)
{
	assert(m_TPM != NULL );
	assert( job != NULL );
	RequestData *JobData = new RequestData;
	JobData->data = job_param;
	JobData->fd = clientfd;
	m_TPM->RunJob(job,(void*)JobData);
}


int /* select and thread */
RequestHandler::handleClient(int serverfd)
{
	int connfd,clientfd;
	SOCKADDR_IN cliAddr;
	socklen_t addrlen;
	//for select
	struct timeval timeout = {0,0};
	int i,max_index,maxfd;
	int nready,client[FD_SETSIZE];
	ssize_t n;
	fd_set rset,allset;

	maxfd = serverfd;
	max_index = -1;
    for(i=0;i<FD_SETSIZE;i++)
      client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(serverfd,&allset);   

	while(true)
	{
		rset = allset;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;	// 3 seconds 
		nready=select(maxfd+1,&rset,NULL,NULL,&timeout);
		//check read fd sets
		if(FD_ISSET(serverfd,&rset))
		{
			//client connect
			addrlen = sizeof(cliAddr);
			if(0>=(connfd = accept(serverfd, (SOCKADDR*)&cliAddr, &addrlen)))
			{
				perror("accept error");
				return -1;
			}
			cout<<"[DEBUG] accept a new client: "<<inet_ntoa(cliAddr.sin_addr)<<":"<<ntohs(cliAddr.sin_port)<<endl;			
			for(i=0;i<FD_SETSIZE;i++)
            {
                if (client[i]<0)
                {
                    client[i]=connfd;
                    break;
                }
            }
            if(i>=FD_SETSIZE)
            {
                cout<<"[DEBUG] too many clients"<<endl;
                close(connfd);
                continue;
            }

            FD_SET(connfd,&allset); 	// add connfd into allset
            if(connfd>maxfd) maxfd=connfd;
            if(i>max_index)       max_index=i;	
            if(--nready<=0)  continue;
		}

        for(i=0;i<=max_index;i++)
        {
        	if( 0 > (clientfd=client[i])) continue;
            if(FD_ISSET(clientfd,&rset))
            {
				//deal with each clientfd
				request_handle(clientfd);
                FD_CLR(clientfd,&allset); 
                client[i]=-1;   
                if(--nready<=0)
            		break;
            }  	
        }
			
	}
}

void 
RequestHandler::request_handle(int clientfd)
{
	int *lptr;
    pthread_t tid;
	lptr=(int*)malloc(sizeof(int));
	*lptr=clientfd;
	if ( 0!=pthread_create(&tid,NULL,threadPerClient,lptr))
	{
		perror("pthread_create");
	}
}

//static method
void* 
RequestHandler::threadPerClient(void *arg)
{
	int connfd=*((int*)arg);
	free(arg);
	pthread_detach(pthread_self());
	RequestHandler::dispatchClientRequest(connfd);
	//close( connfd );	//close by Job not here
	return NULL;
}

//static method
void 
RequestHandler::dispatchClientRequest(int clientfd)
{
    ssize_t n;
    char line[4096]={0};
    char *requestData = NULL;
    //while( (n=read(clientfd,line,4096))>0 )
    if( (n=read(clientfd,line,4096))>0 )
    {
    	cout<<"[DEBUG] recv:"<<line<<endl;
		line[n]=0;
		if(line[n-1] == 0x0a || line[n-1] == 0x0d)
		{
			line[n-1] = 0;
		}
		//requestData = (char*)malloc(strlen(line)+1);
		requestData = (char*)malloc(strlen(line)+1);
		memset(requestData,0,strlen(line)+1);
		strncpy(requestData,line,strlen(line));

		/* dispatch job depends on requestData */
		if( strcasestr(requestData,"sql="))
		{
			char *sql = requestData + strlen("sql=");
			RunJob(clientfd,m_QueryDBJob,sql);
		}
		else if(strcasestr(requestData,"file="))
		{
			char *file = requestData + strlen("file=");
			RunJob(clientfd,m_GetFileJob,file);
		}
		else
		{
			Logger("request data format error!\n");	
			write(clientfd,"request data format error!\n",27);	
			close(clientfd);
		}
    }
    if(n == 0)
    {
    	Logger("client disconnect\n");
    }
}
