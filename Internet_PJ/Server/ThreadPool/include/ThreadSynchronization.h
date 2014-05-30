#ifndef __THREAD__SYNC__
#define __THREAD__SYNC__

#include "Common.h"


//pthread_mutex_t wrapper class
class ThreadMutex
{
    public:
        pthread_mutex_t m_Mutex;
    public:
    ThreadMutex(){ pthread_mutex_init(&m_Mutex, NULL); }
    ~ThreadMutex(){ pthread_mutex_destroy(&m_Mutex); }
    void Lock(){ pthread_mutex_lock(&m_Mutex); }
    int Trylock(){ return pthread_mutex_trylock(&m_Mutex); }
    void Unlock(){ pthread_mutex_unlock(&m_Mutex); }
};

//pthread_cond_t wrapper class
class ThreadCond
{
    public:
        pthread_cond_t m_Cond;
        pthread_mutex_t *m_pMutex;
    public:
        ThreadCond(ThreadMutex *pTMutex)    //bind ThreadCond.m_cond on pTMutex->m_Mutex
        { 
            assert(pTMutex!=NULL);
            assert(&pTMutex->m_Mutex!=NULL);
            m_pMutex = &pTMutex->m_Mutex;
            pthread_cond_init(&m_Cond, NULL); 
        }
        ~ThreadCond(){ pthread_cond_destroy(&m_Cond); }
        void Wait(){ pthread_cond_wait(&m_Cond,m_pMutex); }
        void Signal(){ pthread_cond_signal(&m_Cond); }
        void SignalAll(){ pthread_cond_broadcast(&m_Cond); }
};
#endif
