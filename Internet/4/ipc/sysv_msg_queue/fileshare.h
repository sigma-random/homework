#ifndef __FILE_SHARE__
#define __FILE_SHARE__

#include "sysv_msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>


#define  MODE 		( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define  FLAGS 		(O_CREAT  | O_RDWR)
#define  MAX_BUF_SIZE	4096
#define  MAX_PATH	1024



struct timeval timestart;
struct timeval timeend;
float timeuse;


float getSpendTime(struct timeval timestart ,struct timeval timeend ) {

	return ((1000000*(timeend.tv_sec-timestart.tv_sec) +\
		 timeend.tv_usec-timestart.tv_usec) / 1000000); 
}



int fileclient(int readid, int writeid) {

	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	char newpath[MAX_PATH];
	struct MsgNode msgnode;

	memset(path,0,MAX_PATH);
	memset(&msgnode,0,sizeof(msgnode));

	printf("[Client] input the filepath: ");
	fgets(path, MAX_PATH, stdin);
	len = strlen(path);
	if( '\n' == path[len -1] ) {
		path[len -1] = '\0';
		len--;
	}

	//send filename
	msgnode.len = len;
	msgnode.msgbuf.type = 1;
	strncpy(msgnode.msgbuf.data,path,len);
	if(0 > Msgsend(writeid,&msgnode)) {
		perror("[Client] msgsend");
		return (-1);
	}

	sleep(1);

	// open or create new file //
	memset(newpath,0,MAX_PATH);
	printf("[Client] input new filename: ");
	fgets(newpath, MAX_PATH, stdin);
	len = strlen(newpath);
	if( '\n' == newpath[len -1] ) {
		newpath[len -1] = '\0';
	}

	printf("[Client] save file: \"%s\" to \"%s\" \n",path,newpath);
	if( 0 > (fd = open(newpath,FLAGS , MODE)) ) {
		perror("[Client] open");
		exit (-1);
	}

	//start time
	gettimeofday(&timestart,NULL);

	// recv file content //
	memset(msgnode.msgbuf.data,0,MAX_BUF_SIZE);
	while(1) {
		if ( 0 > (r_cnt = Msgrecv(readid,&msgnode))){
			perror("[Client] msgrecv");
			return (-1);
		}
		if( 0 == msgnode.len){
			printf("[Client] finish!\n");
			break;
		}
		// write content to newfile //
		if(-1 == write(fd,msgnode.msgbuf.data,r_cnt)) {
			perror("[Client] write");
			break;
		}
		memset(msgnode.msgbuf.data,0,MAX_BUF_SIZE);
	}

	close(fd);
	//end time
	gettimeofday(&timeend,NULL);
	printf("[Client] take time: %f s\n",getSpendTime(timestart,timeend)); 
	
	return 0;
}

int fileserver(int readid, int writeid) {
	
	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	struct MsgNode msgnode;

	memset(path,0,MAX_PATH);
	memset(&msgnode,0,sizeof(msgnode));

	if(0 >= (r_cnt = Msgrecv(readid,&msgnode))) {
		if( 0 == r_cnt ) {
			printf("[Server] error: you never give the filepath!\n");
			return (-1);
		}
		perror("[Server] Msgrecv");
		return (-1);
	}
	printf("[Server] read file: %s\n",msgnode.msgbuf.data);
	strncpy(path,msgnode.msgbuf.data,msgnode.len);
	// open file //
	if( 0 > (fd = open(path,O_RDONLY))) {
		perror("[Server] open");
		exit (-1);
	}

	// read file and sent file content //
	memset(msgnode.msgbuf.data,0,MAX_BUF_SIZE);
	while( 0 < (r_cnt = read(fd,msgnode.msgbuf.data,MAX_BUF_SIZE))) {
		msgnode.len = r_cnt;
		msgnode.msgbuf.type = 1;
		if(0 > Msgsend(writeid,&msgnode)) {
			perror("[Server] Msgsend");
			return (-1);			
		}
		memset(msgnode.msgbuf.data,0,MAX_BUF_SIZE);
	}
	/* endding msg block */
	msgnode.len = 0;
	msgnode.msgbuf.type = 1;	
	memset(msgnode.msgbuf.data,0,MAX_BUF_SIZE);
	Msgsend(writeid,&msgnode);

	close(fd);

	return 0;
}

#endif