#include "pipe.h"
#include "fileshare.h"

void signal_handler(int signum) {

    printf("\n[*] detect process[%d] exit!\n",getpid());
    exit(0);
}


int main(int argc, char **argv) {

    pid_t pid = 0;
    int status = 0;
    int cmdPipe[2] = {0};
    int dataPipe[2] = {0};
  
    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);
    signal(SIGSTOP,signal_handler);
    
    Mkpipe(cmdPipe);
    Mkpipe(dataPipe);

	if(-1 == (pid = fork())) {
		perror("[Server] fork");
		return(-1);
	}

	/* child process */
	if(0 == pid) { 

		close(cmdPipe[0]);
		close(dataPipe[1]);
		
		//start fileclient
		fileclient(dataPipe[0],cmdPipe[1]);
		
		
		close(dataPipe[0]);
		close(cmdPipe[1]);

		exit(0);

	}

	/* parent process */
	close(dataPipe[0]);
	close(cmdPipe[1]);

	//start fileserver
	fileserver(cmdPipe[0],dataPipe[1]);

	close(cmdPipe[0]);
	close(dataPipe[1]);
	waitpid(pid,&status,0);


	return(0);

}
