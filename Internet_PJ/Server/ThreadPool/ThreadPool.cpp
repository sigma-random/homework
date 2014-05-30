#include "include/ThreadPool.h"


#define MAX_THREAD_NUM  100
#define MIN_THREAD_NUM  1
#define MAX_ACTIVE_NUM  20
#define MIN_ACTIVE_NUM  10

ThreadPool::ThreadPool(int initnum) 
{ 
    printf("[DEBUG] Init thread pool!\n");
    assert(initnum>=MIN_THREAD_NUM && initnum<=MAX_THREAD_NUM); 
    m_InitNum  = initnum; 
    printf("[DEBUG] m_InitNum = %d\n",initnum);

    m_MaxNum   = MAX_THREAD_NUM;  
    printf("[DEBUG] m_MaxNum = %d\n",m_MaxNum);

    assert(MAX_ACTIVE_NUM<MAX_THREAD_NUM);
    m_MaxActiveNum = MAX_ACTIVE_NUM <= initnum ? initnum : MAX_ACTIVE_NUM;
    printf("[DEBUG] m_MaxActiveNum = %d\n",m_MaxActiveNum);
    
    m_MinActiveNum = MIN_ACTIVE_NUM <= MAX_ACTIVE_NUM ? MIN_ACTIVE_NUM : MAX_ACTIVE_NUM;
    printf("[DEBUG] m_MinActiveNum = %d\n",m_MinActiveNum);
    m_CurIdleNum = 0;

    InitMutexAndCond();
    sleep(1);

    m_RunningList.clear(); 
    m_IdleList.clear(); 

    for(int i=0;i< m_InitNum;i++)
    {
        WorkerThread* thread = new WorkerThread(); 
        AppendToIdleList(thread); 
        thread->SetThreadPool(this); 
        thread->Start(thread);    //begin the thread,the thread wait for job
        usleep(100);
    } 
    printf("[DEBUG] Set up thread pool!\n");
} 

ThreadPool::~ThreadPool() 
{ 
   TerminateAll(); 
   DestroyMutexAndCond();
} 

void ThreadPool::InitMutexAndCond()
{
    printf("[DEBUG] ThreadPool::InitMutexAndCond()\n");  

    m_pJobTMutex = new ThreadMutex();    
    assert(m_pJobTMutex != NULL);
    m_pJobTCond  = new ThreadCond(m_pJobTMutex);    //bind m_pJobTCond on m_pJobTMutex
    assert(m_pJobTCond != NULL);

    m_pRunningMutex = new ThreadMutex();
    assert(m_pRunningMutex != NULL);    
    m_pRunningCond  = new ThreadCond(m_pRunningMutex);
    assert(m_pRunningCond != NULL);    

    m_pIdleMutex = new ThreadMutex();   
    assert(m_pIdleMutex != NULL); 
    m_pIdleCond  = new ThreadCond(m_pIdleMutex);
    assert(m_pIdleCond != NULL);   

}

void ThreadPool::DestroyMutexAndCond()
{
    printf("[DEBUG] ThreadPool::DestroyMutexAndCond()\n");  
    
    delete m_pJobTMutex;
    delete m_pJobTCond;

    delete m_pRunningMutex;
    delete m_pRunningCond;

    delete m_pIdleMutex; 
    delete m_pIdleCond;  

}

void ThreadPool::DebugPoolInfo()
{
    

}

void ThreadPool::TerminateAll() 
{ 
/*
    m_pRunningMutex->Lock(); 
    for(int i=0;i < m_RunningList.size();i++) 
    { 
        WorkerThread* thread = m_RunningList[i]; 
        thread->Join(); 
    } 
    m_pRunningMutex->Unlock(); 

    m_pIdleMutex->Lock(); 
    for(int i=0;i < m_IdleList.size();i++) 
    { 
        WorkerThread* thread = m_IdleList[i]; 
        thread->Join(); 
    } 
    m_pIdleMutex->Unlock(); 
*/

    return; 
} 
 
WorkerThread* ThreadPool::GetIdleThread(void) 
{ 
    m_pIdleMutex->Lock(); 
    while(m_IdleList.size() == 0 ) 
    {
        m_pIdleCond->Wait(); 
        break;
    }
    WorkerThread* thread = (WorkerThread*)m_IdleList.front(); 
    printf("[DEBUG] ThreadPool::GetIdleThread():get an idle thread, id is:%lu\n",thread->GetThreadID());
    m_pIdleMutex->Unlock(); 
    return thread; 
} 
 
//add an idle thread to idle list 
void ThreadPool::AppendToIdleList(WorkerThread* jobthread) 
{ 
    m_pIdleMutex->Lock(); 
    m_IdleList.push_back(jobthread); 
    m_CurIdleNum++;
    //printf("[DEBUG] ThreadPool::AppendToIdleList():idle thread list number:%d\n",m_CurIdleNum);
    m_pIdleMutex->Unlock(); 
    m_pIdleCond->Signal();
} 
 
//move an idle thread to busy thread 
void ThreadPool::MoveToRunningList(WorkerThread* idlethread) 
{ 
    m_pRunningMutex->Lock(); 
    m_RunningList.push_back(idlethread); 
    printf("[DEBUG] ThreadPool::MoveToRunningList(): Running Thread list number:%d\n",GetRunningNum());
    m_pRunningMutex->Unlock(); 
    
    m_pIdleMutex->Lock(); 
    vector<WorkerThread*>::iterator pos; 
    pos = find(m_IdleList.begin(),m_IdleList.end(),idlethread); 
    if(pos != m_IdleList.end())
    { 
        m_IdleList.erase(pos);
        m_CurIdleNum--;  
    }
    printf("[DEBUG] ThreadPool::MoveToRunningList(): idle thread list number:%d\n",m_CurIdleNum);
    m_pIdleMutex->Unlock(); 
} 

void ThreadPool::MoveToIdleList(WorkerThread* RunningThread) 
{  
    m_pIdleMutex->Lock(); 
    m_IdleList.push_back(RunningThread); 
    m_CurIdleNum++; 
    printf("[DEBUG] ThreadPool::MoveToIdleList(): idle thread list number:%d\n",m_CurIdleNum);
    m_pIdleMutex->Unlock();
    m_pIdleCond->Signal();
    
    m_pRunningMutex->Lock(); 
    vector<WorkerThread*>::iterator pos; 
    pos = find(m_RunningList.begin(),m_RunningList.end(),RunningThread); 
    if(pos!=m_RunningList.end()) 
    {
        m_RunningList.erase(pos);
        printf("[DEBUG] ThreadPool::MoveToIdleList(): Running Thread list number:%d\n",GetRunningNum());
    }
    m_pRunningMutex->Unlock(); 
    m_pRunningCond->Signal();
    
    if(GetCurIdleNum() > GetMaxActiveNum()) 
    {
        printf("[DEBUG] detect too much idle threads : idle_thread_num[%d] > max_active_num[%d]\n\n",GetCurIdleNum(),GetMaxActiveNum());
        DeleteIdleThread(GetCurIdleNum() - GetMaxActiveNum());
    }
        
} 


//create num idle thread and put them to idlelist 
void ThreadPool::CreateIdleThread(int num) 
{ 
    printf("[DEBUG] ThreadPool::CreateIdleThread(): create %d idle threads for need\n",num);
    for(int i=0;i<num;i++)
    { 
        WorkerThread* thread = new WorkerThread(); 
        thread->SetThreadPool(this); 
        AppendToIdleList(thread); 
        thread->Start(thread); //begin the thread,the thread wait for job        
    } 
} 

void ThreadPool::DeleteIdleThread(int num) 
{ 
    printf("[DEBUG] ThreadPool::DeleteIdleThread():delete %d idle threads for no need\n",num);
    for(int i=0;i<num;i++)
    { 
        WorkerThread* thr = GetIdleThread(); 
        m_pIdleMutex->Lock(); 
        vector<WorkerThread*>::iterator pos; 
        pos = find(m_IdleList.begin(),m_IdleList.end(),thr); 
        if(pos!=m_IdleList.end())
        { 
            m_IdleList.erase(pos);
            m_CurIdleNum--;
        }
        m_pIdleMutex->Unlock();
    }   
} 
 
void ThreadPool::RunJob(Job* job,void* job_param) 
{ 
    assert(job!=NULL); 

    m_pRunningMutex->Lock();
    while(GetRunningNum() == m_MaxActiveNum)  
    {
        printf("[DEBUG] busy thread num up to m_MaxActiveNum[%d]\n",m_MaxActiveNum);
        m_pRunningCond->Wait();  
        printf("[DEBUG] busy thread num down, m_MaxActiveNum[%d]\n",GetRunningNum()); 
        break;   
    }
    m_pRunningMutex->Unlock();

    m_pIdleMutex->Lock();
    if(m_IdleList.size()<m_MinActiveNum && (unsigned int)GetAllNum() < m_MaxNum)
    { 
        m_pIdleMutex->Unlock();
        printf("[DEBUG] ThreadPool::RunJob(): begin to create more thread.\n");
        CreateIdleThread(m_MinActiveNum-m_IdleList.size());
    }
    m_pIdleMutex->Unlock();

    WorkerThread*  idlethread = GetIdleThread(); 
    if(idlethread != NULL) 
    { 
        MoveToRunningList(idlethread); 
        this->cur_running_tid  = idlethread->GetThreadID();
        idlethread->SetThreadPool(this); 
        job->SetWorkThread(idlethread); 
        printf("[DEBUG] ThreadPool::RunJob(): assigned a Job to thread[%lu]\n", idlethread->GetThreadID()); 
        idlethread->SetJob(job,job_param); 
    } 
} 

