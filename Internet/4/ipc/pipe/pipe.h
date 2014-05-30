
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



int Mkpipe(int pipefd[]) {

	if (pipe(pipefd) == -1) {
		perror("[Server] pipe");
		exit(-1);
	}
	return (0);
}




#endif