#ifndef __THREAD__
#define __THREAD__

#include "Common.h"

typedef enum{

    THREAD_RUNNING=1,
    THREAD_WAITTING=2

}ThreadState;


class Thread 
{ 

public: 
    Thread(); 
    Thread(bool createsuspended,bool detach){}; 
    virtual ~Thread(); 
    bool    Start(Thread* thread);
    virtual void RunJob() = 0; 

    ThreadState GetThreadState(){return m_ThreadState;} 
    void SetThreadState(ThreadState state){m_ThreadState = state;}
   
    unsigned long GetThreadID(void){return m_ThreadID;} 
   
    bool SetPriority(int priority); 
    int GetPriority(void); 
   
    bool Detach(void); 
    void Join(); 
    bool Terminate();
    void Exit(); 
    bool Wakeup(); 
   
protected: 
    static void* ThreadProxyFunc(void*); 
 
private: 
    unsigned long m_ThreadID;    
    ThreadState m_ThreadState;      //the state of the thread 

}; 

#endif
