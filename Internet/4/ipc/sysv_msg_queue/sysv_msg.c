#include "sysv_msg.h"
#include "fileshare.h"


int cmd_msgid = 0;
int data_msgid = 0;

void signal_handler(int signum) {

    printf("\n[*] detect process[%d] exit!\n",getpid());
 	rmmsg(cmd_msgid);
	rmmsg(data_msgid);
    exit(0);
}


int main(int argc, char **argv) {

    pid_t pid = 0;
    int status = 0;
    
    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);
    signal(SIGSTOP,signal_handler);
    
    cmd_msgid = Msgget(CMD_KEY,SVMSG_FLAG);
    data_msgid = Msgget(DATA_KEY,SVMSG_FLAG);
    
	if(-1 == (pid = fork())) {
		perror("fork");
		return(-1);
	}

	/* child process */
	if(0 == pid) { 

		//start fileclient
		fileclient(data_msgid,cmd_msgid);
		exit(0);
	}

	/* parent process */
	//start fileserver
	fileserver(cmd_msgid,data_msgid);

	waitpid(pid,&status,0);
	rmmsg(cmd_msgid);
	rmmsg(data_msgid);
	return(0);
}

