#ifndef __FILE_SHARE__
#define __FILE_SHARE__

#include "sysv_shm.h"
#include <fcntl.h>
#include <sys/time.h>
#include <semaphore.h>

#define  MODE 		( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define  FLAGS 		(O_CREAT  | O_RDWR)
#define  MAX_BUF_SIZE	4096
#define  MAX_PATH	256


struct timeval timestart;
struct timeval timeend;
float timeuse;


float getSpendTime(struct timeval timestart ,struct timeval timeend ) {

	return ((1000000*(timeend.tv_sec-timestart.tv_sec) +\
		 timeend.tv_usec-timestart.tv_usec) / 1000000); 
}


int fileclient(int shmid) {

	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	char newpath[MAX_PATH];
	pMsgNode ptrMsgNode;
	char *ptr;
	sem_t *readsem;
	sem_t *writesem;

    ptr = (char*)Shmat(shmid);
    //printf("[Server] shared mem addr is 0x%08x\n",ptr);

    ptrMsgNode = (pMsgNode)ptr;
    readsem = &(ptrMsgNode->read_sem);
    writesem = &(ptrMsgNode->write_sem);
   
	memset(path,0,MAX_PATH);
	printf("[Client] input the filepath: ");
	fgets(path, MAX_PATH, stdin);
	len = strlen(path);
	if( '\n' == path[len -1] ) {
		path[len -1] = '\0';
		len--;
	}

	sem_wait(writesem);

	//send filename
	ptrMsgNode->len = len;
	strncpy(ptrMsgNode->data,path,len);
	sem_post(readsem);
	sem_post(writesem);

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
	sem_wait(readsem);
	while(1){
		if(0 < ptrMsgNode->len) {
			write(fd,ptrMsgNode->data,ptrMsgNode->len);
		}
		if(0 == ptrMsgNode->len){
			break;
		}
		sem_post(writesem);
		sem_wait(readsem);
	}

	close(fd);

	//end time
	gettimeofday(&timeend,NULL);

	printf("[Client] take time: %f s\n",getSpendTime(timestart,timeend)); 

	return 0;
}



int fileserver(int shmid) {
	
	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	pMsgNode ptrMsgNode;
	char *ptr;
	sem_t *readsem;
	sem_t *writesem;


    ptr = (char*)Shmat(shmid);
    //printf("[Server] shared mem addr is 0x%08x\n",ptr);

    ptrMsgNode = (pMsgNode)ptr;
    readsem = &(ptrMsgNode->read_sem);
    writesem = &(ptrMsgNode->write_sem);

	sem_init(readsem,1,0);
	sem_init(writesem,1,1);

	sem_wait(readsem);

	//recv filename
	if(ptrMsgNode->len > 0) {
		//printf("[Server] filepath: %s\n",(ptrMsgNode->data));
	}
	memset(path,0,MAX_PATH);
	strncpy(path,ptrMsgNode->data,ptrMsgNode->len);
	/* open file */
	if( 0 > (fd = open(path,O_RDONLY))) {
		perror("[Server] open");
		exit (-1);
	}

	// read file and sent file content //
	sem_wait(writesem);
	while(1) {
		memset(ptrMsgNode->data,0,MAX_BUF_SIZE);
		if(0 > (r_cnt = read(fd,ptrMsgNode->data,MAX_BUF_SIZE))){
			perror("[Server] read");
			break;
		}
		ptrMsgNode->len = r_cnt;
		if( 0 == r_cnt) {
			sem_post(readsem);
			sem_post(writesem);
			break;
		}
		sem_post(readsem);
		sem_wait(writesem);
	}
	
	close(fd);
	shmdt(ptr);
	return 0;
}

#endif