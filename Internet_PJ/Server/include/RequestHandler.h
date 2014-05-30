#ifndef __REQUEST_HANDLER__
#define __REQUEST_HANDLER__

#include "Common.h"
#include "TcpSocketHandler.h"
#include "Logger.h"
#include "../ThreadPool/include/Job.h"
#include "../ThreadPool/include/ThreadPoolManage.h"

// implement TcpSocketHandler Interface
class RequestHandler :public TcpSocketHandler
{

public:
	RequestHandler();
	virtual ~RequestHandler();
	int    handleClient(int serverfd);
	void   request_handle(int clientfd);
	static void  dispatchClientRequest(int clientfd);
	static void* threadPerClient(void*);
	void   BindThreadPool(ThreadPoolManage* TPM);
	void   BindDBServiceJob(Job* clientJob);
	void   BindFileServiceJob(Job* clientJob);
	static void  RunJob(int clientfd, Job* job, char *job_param);

public:
	static Job* m_QueryDBJob;
	static Job* m_GetFileJob;
	static ThreadPoolManage* m_TPM;
};

#endif