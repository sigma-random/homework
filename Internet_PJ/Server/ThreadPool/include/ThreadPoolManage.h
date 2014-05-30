#ifndef __THREAD_POOL_MANAGE__
#define __THREAD_POOL_MANAGE__

#include "ThreadPool.h"
#include "Job.h"

class ThreadPoolManage 
{ 

public: 
    ThreadPoolManage (int num = 20); 
    virtual ~ThreadPoolManage (); 
    void	RunJob(Job* job,void* job_params); 
    void	TerminateAll(void);
    void	SetThreadNum(int num);

private: 
    ThreadPool*	m_ThreadPool; 
    int			m_ThreadNum;

};

#endif
