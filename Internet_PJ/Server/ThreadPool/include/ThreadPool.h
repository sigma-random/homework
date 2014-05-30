#ifndef __ThREAD_POOL__
#define __ThREAD_POOL__

#include "Common.h"
#include "Job.h"
#include "WorkerThread.h"
#include "ThreadSynchronization.h"

class Job;
class WorkerThread;

class ThreadPool
{ 

public: 

    ThreadPool(int initnum=10); 
    virtual ~ThreadPool();  
    
    void    InitMutexAndCond();

    void    DestroyMutexAndCond();

    void    TerminateAll(); 
    
    void    RunJob(Job* job,void* job_param); 
    

    void    AppendToIdleList(WorkerThread* jobthread); 
    void    MoveToRunningList(WorkerThread* idlethread); 
    void    MoveToIdleList(WorkerThread* busythread); 
    void    DeleteIdleThread(int num);     
    void    CreateIdleThread(int num); 
    WorkerThread* GetIdleThread();

    void    SetMaxNum(int maxnum){m_MaxNum = maxnum;} 
    int     GetMaxNum(){return m_MaxNum;} 
    void    SetMinActiveNum(int minnum){m_MinActiveNum = minnum;} 
    int     GetMinActiveNum(){return m_MinActiveNum;} 
    void    SetMaxActiveNum(int highnum){m_MaxActiveNum = highnum;} 
    int     GetMaxActiveNum(){return m_MaxActiveNum;} 

    int     GetCurIdleNum(){return m_CurIdleNum;} 
    int     GetRunningNum(){return m_RunningList.size();} 
    int     GetAllNum(){return m_RunningList.size() + m_CurIdleNum;}

    void    DebugPoolInfo();
    
public: 
    
    unsigned long cur_running_tid;  //cunrrent running thread's id

    ThreadMutex *m_pJobTMutex;      // mutex for Running Job Thread
    ThreadCond  *m_pJobTCond;       // cond  for Running Job Thread

    ThreadMutex *m_pRunningMutex;   //for Running List
    ThreadCond  *m_pRunningCond;    

    ThreadMutex *m_pIdleMutex;      //for Idle List
    ThreadCond  *m_pIdleCond;  

private:
    vector<WorkerThread*>   m_RunningList;    //running thread List 
    vector<WorkerThread*>   m_IdleList;        //Idle thread List 

    unsigned int m_InitNum;      //Normal thread num; 
    unsigned int m_MaxNum;       //the max thread num that can create at the same time 
    unsigned int m_MinActiveNum;     //The min num of idle thread that shoule kept 
    unsigned int m_MaxActiveNum; //The max num of idle thread that kept at the same time 
    unsigned int m_CurIdleNum;     //the normal thread num of idle num; 
    
}; 

#endif 
