#ifndef __WORK_THREAD__
#define __WORK_THREAD__

#include "Common.h"
#include "Thread.h"
#include "Job.h"
#include "ThreadPool.h"

class ThreadPool;
class Job ;

class WorkerThread : public Thread 
{

public:
    WorkerThread();
    virtual ~WorkerThread();

    void RunJob();
    void SetJob(Job* job,void* jobdata);
    Job* GetJob(void){return m_Job;}
    
    void SetThreadPool(ThreadPool* pool);
    ThreadPool* GetThreadPool(void){return m_ThreadPool;}

private:
    ThreadPool* m_ThreadPool;
    Job* m_Job;
    void* m_JobData;
};

#endif

