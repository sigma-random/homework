#include "fifo.h"
#include "fileshare.h"

void signal_handler(int signum) {

    printf("\n[*] detect process[%d] exit!\n",getpid());
    exit(0);
}


int main(int argc, char **argv) {

	int readfd = 0;
	int writefd = 0;
    pid_t pid = 0;
    int status = 0;
  
    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);
    signal(SIGSTOP,signal_handler);
    
    if( -1 == Mkfifo(CMD_FIFO,FIFO_MODE)) {
		return(-1);
	}

	if( -1 == Mkfifo(DATA_FIFO,FIFO_MODE)) {
		return(-1);
	}

	if(-1 == (pid = fork())) {
		perror("fork");
		return(-1);
	}

	/* child process */
	if(0 == pid) { 
		// may be block , wait for other process to open pipe by read-mode
		if( -1 == (writefd = open(CMD_FIFO,O_WRONLY))) {
			perror("open");
			return(-1);
		}
		// may be block , wait for other process to open pipe by write-mode
		if( -1 == (readfd = open(DATA_FIFO,O_RDONLY))) {
			perror("open");
			return(-1);
		}

		//start fileclient
		fileclient(readfd,writefd);
		close(readfd);
		close(writefd);

		exit(0);
	}

	/* parent process */

	atexit(rmfifo);
    
    // may be block , wait for other process to open pipe by write-mode
	if( -1 == (readfd = open(CMD_FIFO,O_RDONLY))) {
		perror("open");
		return(-1);
	}
	// may be block , wait for other process to open pipe by read-mode
	if( -1 == (writefd = open(DATA_FIFO,O_WRONLY))) {
		perror("open");
		return(-1);
	}
	
	//start fileserver
	fileserver(readfd,writefd);
	close(readfd);
	close(writefd);

	waitpid(pid,&status,0);
	return(0);
}
