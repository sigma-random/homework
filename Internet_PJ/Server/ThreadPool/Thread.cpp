#include "include/Thread.h"


Thread::Thread() {}

Thread::~Thread(){}

void* Thread::ThreadProxyFunc(void* param) 
{
    Thread* t = (Thread*) param;
    t->RunJob();
    return NULL;    
}

bool Thread::Start(Thread* thread)
{
    if(0 != pthread_create(&this->m_ThreadID, NULL, ThreadProxyFunc, (void*)thread))
    {
        perror("pthread_create");
        return false;
    }
    assert(m_ThreadID>0);
    //cout<<"[DEBUG] Thread::Create a new thread["<<m_ThreadID<<"]"<<endl;

    return true;
}

void Thread::Join()
{
    //printf("\n[IMPORT] thread[%lu] joined\n",m_ThreadID);
    pthread_join(m_ThreadID,NULL);
}

void Thread::Exit()
{
    printf("\n[IMPORT] thread[%lu] exit\n",m_ThreadID);
    pthread_exit(NULL);
}
