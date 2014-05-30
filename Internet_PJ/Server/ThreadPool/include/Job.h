#ifndef __JOB__
#define __JOB__

#include "Common.h"
#include "WorkerThread.h"

class WorkerThread;

class Job 
{ 
public: 
    WorkerThread  *m_WorkThread;
public: 
    Job(){}
    virtual ~Job(){}
    WorkerThread *GetWorkThread(void){ return m_WorkThread; }
    void    SetWorkThread ( WorkerThread *WorkThread ){ m_WorkThread = WorkThread; }
    virtual void RunJob(void *job_param)=0; 
}; 

#endif

