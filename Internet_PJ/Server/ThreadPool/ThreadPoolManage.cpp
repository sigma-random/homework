#include "include/ThreadPoolManage.h"

ThreadPoolManage::ThreadPoolManage(int num)
{ 
    m_ThreadNum = num==0?10:num; 
    cout<<"\n\n[DEBUG] Init the ThreadPool which contains "<<num<<" threads"<<endl;
    m_ThreadPool = new ThreadPool(m_ThreadNum); 
} 

ThreadPoolManage::~ThreadPoolManage()
{ 
    if(NULL != m_ThreadPool) 
        delete m_ThreadPool; 
    m_ThreadPool = NULL;
} 

void ThreadPoolManage::SetThreadNum(int num)
{ 
    m_ThreadNum = num; 
} 
 
void ThreadPoolManage::RunJob(Job* job, void* job_params)
{ 
    cout<<"[DEBUG] ThreadPoolManage::RunJob()..."<<endl;
    return m_ThreadPool->RunJob(job,job_params); 
} 
 
void ThreadPoolManage::TerminateAll(void)
{ 
    cout<<"[DEBUG] ThreadPoolManage::TerminateAll()..."<<endl;
    assert(m_ThreadPool!=NULL);
    m_ThreadPool->TerminateAll(); 
} 
