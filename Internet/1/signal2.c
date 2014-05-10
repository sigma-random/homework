#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/time.h>

#define TIMEOUT 5


static time_t timer;

static void sig_handler(int signum)
{
    switch(signum)
    { 
        case SIGUSR1:
            timer = time(NULL);
            printf("SIGUSR1: %s\n",ctime(&timer));
            break;
        case SIGUSR2:
            printf("SIGUSR2: recieve SIGUSR2\n");
            break;
        case SIGCHLD:
            printf("SIGCHLD: exit child process\n");
            break;
        default:
            printf("get signal: %d\n",signum);
            break;
    }
}


int main(int argc, char** argv)
{
    pid_t pid,ppid;
    int timeout;
    struct sigaction sigact;
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask,SIGUSR1);
    sigaddset(&sigact.sa_mask,SIGUSR2);
    sigaddset(&sigact.sa_mask,SIGCHLD);
    sigact.sa_flags = SA_NODEFER;
    sigact.sa_handler = sig_handler;
    sigaction(SIGUSR1,&sigact,NULL);
    sigaction(SIGUSR2,&sigact,NULL);
    sigaction(SIGCHLD,&sigact,NULL);

    if((pid = fork()) < 0)
    {
        perror("fork failed!\n");
        exit(-1);
    }
    if(0==pid)  //child process
    {
        timeout = TIMEOUT; 
        if((ppid = getppid()) <= 0)   //get parent pid
        {
            perror("getppid() failed!\n");
            exit(-1);
        }
        printf("ppid:%d\n\n\n", ppid);
        while( timeout-- )
        {
            sleep(1);       //the result of sending signal SIGUSR1 after sleep(1) could  differ from sending signal SIGUSR1 before sleep(1)
            printf("timeout:%d\n", timeout);
            kill(ppid,SIGUSR1);  //send signal:SIGUSR1 to parent
            //sleep(1);
        }
        kill(ppid,SIGUSR2);  //send signal:SIGUSR2 to parent
        exit(0);
    }
    while(1)
    {
        sleep(1);
    }
    //parent process
    /*
    if(waitpid(pid,NULL,0) < 0)    //wait until child process exit
    {
        perror("waitpid error\n");
        exit(-3);
    }
    */
    printf("exit parent process!\n");
    return 0;
}

//random

