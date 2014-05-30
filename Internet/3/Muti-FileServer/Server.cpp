#include "common.h"

using namespace std;



extern int startFileServer(const char*, int );


void sighandler(int signum) {

    int status;
    if(signum == SIGCHLD) {
        cout<<"\ndetect a child process exiting!\n"<<endl;
        while(waitpid(-1,&status,WNOHANG));
    }

}
int main(int argc, char** argv) {
    
    
    if( argc != 3) {
        cout<<"[-] usaege:  "<<argv[0]<<"  ip  port\n";
        exit(-1);
    }

    signal(SIGCHLD,sighandler);

    startFileServer(argv[1], atoi(argv[2]));
    return 0;
}

//:end
