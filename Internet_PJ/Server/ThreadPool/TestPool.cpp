
#include "include/Common.h"
#include "include/Job.h"
#include "include/ThreadPoolManage.h"


class CXJob:public Job 
{ 
public: 
    
    CXJob(){} 
    
    ~CXJob(){} 

    void RunJob(void* job_param)
    { 
        int i = job_param != NULL?*(int*)job_param:-2;
        i++;
        printf("\n\n[DEBUG] Finish Job[%d], thread[%lu]\n\n",i,pthread_self()); 
        usleep(500);
    } 
};

int main(void) 
{ 
    ThreadPoolManage* manage = new ThreadPoolManage(10); 
    for(int i=0;i<50;i++)
    { 
        CXJob* job = new CXJob();
        int *tmp = (int*)malloc(sizeof(int));
        *tmp = i;
        manage->RunJob(job,tmp);

    } 
    getchar();
    cout<<"[DEBUG] ^^^^^^^^^^^^^^^^^^^^^^^ Run Finished ^^^^^^^^^^^^^^^^^"<<endl;
    getchar();

/*    
    for(int i=0;i<20;i++)
    { 
        manage->RunJob(new CXJob(),NULL);
    } 
 */
    delete manage;
 
    return 0;

}

