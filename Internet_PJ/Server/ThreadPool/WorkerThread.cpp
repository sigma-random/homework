#include "include/WorkerThread.h"


WorkerThread::WorkerThread()
{
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
}

WorkerThread::~WorkerThread()
{
/*
    if(m_Job != NULL)
        delete m_Job;
    if(m_JobData != NULL)
        delete m_JobData;
*/
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
}

void WorkerThread::RunJob()
{
    
    for(;;)
    {
        m_ThreadPool->m_pJobTMutex->Lock();
        while(m_Job == NULL)
        {
            //printf("[DEBUG] WorkerThread::thread[%lu] wait for a Job.\n",GetThreadID());
            m_ThreadPool->m_pJobTCond->Wait();
            if(this->m_ThreadPool->cur_running_tid == GetThreadID())
            {
                if(m_Job != NULL)   // check m_Job again
                {
                    printf("[DEBUG] thread[%lu] run again!!\n",GetThreadID());
                    break;          //wake up
                }
            }
        }
        printf("[DEBUG] WorkerThread::RunJob() thread[%lu]...\n",GetThreadID());
        assert(m_Job != NULL);   
        m_Job->RunJob(m_JobData);
        m_Job->SetWorkThread(NULL);
        m_Job = NULL;
        m_ThreadPool->m_pJobTMutex->Unlock();
        m_ThreadPool->MoveToIdleList(this);
    }
}

void WorkerThread::SetJob(Job* job,void* job_param)
{
    m_Job = job;
    m_JobData = job_param;
    m_Job->SetWorkThread(this);
    //wake up all idle treads , it makes much costs than pthread_cond_signal
    m_ThreadPool->m_pJobTCond->SignalAll();
}

void WorkerThread::SetThreadPool(ThreadPool* pool)
{
    m_ThreadPool = pool;
}