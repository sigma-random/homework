
#include "sysv_shm.h"
#include "fileshare.h"

//global
int shmid = 0;

void signal_handler(int signum) {

    printf("\n[*] detect process[%d] exit!\n",getpid());
    rmshm(shmid);
}


int main(int argc, char **argv) {

    pid_t pid = 0;
    int status = 0;

    
    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);
    signal(SIGSTOP,signal_handler);
    
    shmid = Shmget(SHM_KEY,SVSHM_FLAG);


    if(-1 == (pid = fork())) {
        perror("fork");
        return(-1);
    }

    /* child process */
    if(0 == pid) { 
        
        //start fileclient
        fileclient(shmid);
        exit(0);
    }

    /* parent process */
    //start fileserver
    fileserver(shmid);

    waitpid(pid,&status,0);

    rmshm(shmid);
    return 0;
}


