#ifndef __SV_SHARED_MEN__
#define __SV_SHARED_MEN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>


#define  SVSHM_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define  SVSHM_FLAG (IPC_CREAT | SVSHM_MODE )

#define  BUF_SIZE	4096

/* shared mem key */
#define  SHM_KEY	1234567L


typedef struct MsgNode* pMsgNode;

struct MsgNode {
	sem_t read_sem;
	sem_t write_sem;
	long len;
	char data[BUF_SIZE];
};

/* shmget wraaper function */
int Shmget(key_t key,int shmflag) {

	int shmid = 0;
    if ( 0 > (shmid = shmget(key, sizeof(struct MsgNode), shmflag)) ) {
        perror("[Server] shmget");
        return(1);
    }
	//printf("[Server] shmid = %d\n",shmid);

	return shmid;
}

void* Shmat(int shmid) {

	void *p = NULL;
	;
	if(0 > (p = shmat(shmid, NULL, 0)) ) {
		perror("[Server] shmat");
		return (void*)(-1);
	}
	return p;
}

int Shmdt(const void *shmaddr) {

	if(0 > shmdt(shmaddr)) {
		return (-1);
	}	
	return 0;
}


int rmshm(int shmid) {

	if(shmid > 0) {
		/* remove shared mem from kernel */
		if( 0 > shmctl(shmid,IPC_RMID,NULL) ) {
			return (-1);
		}
	}
	return shmid;
}

#endif