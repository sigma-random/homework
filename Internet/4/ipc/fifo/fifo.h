
#ifndef  __FIFO__
#define  __FIFO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/wait.h>


#define  CMD_FIFO	"cmd_fifo"
#define  DATA_FIFO	"data_fifo"

#define  FIFO_MODE ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)




int Mkfifo(const char *fifoname, mode_t mode) {

	if(-1 == mkfifo(fifoname, mode) &&  errno != EEXIST) {
		perror("mkfifo");
		return (-1);
	}
	if(errno == EEXIST) {
		printf("fifo: %s exists!\n",fifoname);
	}
	return (0);
}

void rmfifo(void) {
	
	if(-1 == unlink(CMD_FIFO)) {
			perror("unlink");
	}
	//printf("[Server] rmfifo: %s\n",CMD_FIFO);
	
	if(-1 == unlink(DATA_FIFO)) {
			perror("unlink");
	}
	//printf("[Server] rmfifo: %s\n",DATA_FIFO);
}




#endif