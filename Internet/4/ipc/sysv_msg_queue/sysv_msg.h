#ifndef __SV_MSG_QUEUE__
#define __SV_MSG_QUEUE__

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


#define  SVMSG_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define  SVMSG_FLAG (IPC_CREAT | SVMSG_MODE )

#define BUF_SIZE 4096

/* msg queue key */
#define  CMD_KEY	1234567L
#define  DATA_KEY 	8765432L


struct MsgBuf {
	long type;
	char data[BUF_SIZE];
};

struct MsgNode {
	long len;
	struct MsgBuf msgbuf;
};



/* msgget wraaper function */
int Msgget(key_t key,int msgflag) {

	int msqid = 0;

	if(0 > (msqid = msgget(key,msgflag)) ) {
		perror("[Server] msgget");
		return (-1);
	}
	
	//printf("[Server] msqid = %d\n",msqid);

	return msqid;
}

ssize_t Msgsend(int msqid, struct MsgNode* msgnode) {

	ssize_t send_size = 0;
	if(0 > (send_size = msgsnd(msqid,&(msgnode->msgbuf),msgnode->len,0))) {
		return (-1);
	}
	return send_size;
}

ssize_t Msgrecv(int msqid, struct MsgNode* msgnode) {
	ssize_t recv_size = 0;
	if(0 > (recv_size = msgrcv(msqid,&(msgnode->msgbuf),BUF_SIZE,msgnode->msgbuf.type,0))) {
		return (-1);
	}
	msgnode->len = recv_size;
	return recv_size;
}


int rmmsg(int msqid) {

	if(msqid > 0) {
		/* remove msg queue from kernel */
		if( 0 > msgctl(msqid,IPC_RMID,NULL) ) {
			return (-1);
		}
	}
	return msqid;
}

#endif