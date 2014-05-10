#ifndef __FILE_SHARE__
#define __FILE_SHARE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/time.h>

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




int fileclient(int readfd, int writefd) {

	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	char newpath[MAX_PATH];
	char buf[MAX_BUF_SIZE];

	memset(path,0,MAX_PATH);

	printf("[Client] input the filepath: ");
	fgets(path, MAX_PATH, stdin);
	len = strlen(path);
	if( '\n' == path[len -1] ) {
		path[len -1] = '\0';
		len--;
	}

	//write filename
	write(writefd,path,len);

	sleep(1);

	/* open or create new file */
	memset(newpath,0,MAX_PATH);
	printf("[Client] input new filename: ");
	fgets(newpath, MAX_PATH, stdin);
	len = strlen(newpath);
	if( '\n' == newpath[len -1] ) {
		newpath[len -1] = '\0';
	}
	
	//snprintf(path,MAX_PATH,"./fifo_tmp",getpid());
	printf("[Client] save file: \"%s\" to \"%s\" \n",path,newpath);
	if( 0 > (fd = open(newpath,FLAGS , MODE)) ) {
		perror("[Client] open");
		exit (-1);
	}

	//start time
	gettimeofday(&timestart,NULL);

	/* recv file content */
	memset(buf,0,MAX_BUF_SIZE);

	while( 0 < (r_cnt = read(readfd,buf,MAX_BUF_SIZE))) {
		//write(STDOUT_FILENO,buf,r_cnt);
		/* write content to newfile */
		if(-1 == write(fd,buf,r_cnt)) {
			perror("[Client] write");
			break;
		}
		memset(buf,0,MAX_BUF_SIZE);
	}

	close(fd);
	//end time
	gettimeofday(&timeend,NULL);

	printf("[Client] take time: %f s\n",getSpendTime(timestart,timeend)); 
	
	return 0;
}

int fileserver(int readfd, int writefd) {
	
	int fd = 0;
	int len = 0;
	ssize_t r_cnt = 0;
	ssize_t w_cnt = 0;
	char path[MAX_PATH];
	char buf[MAX_BUF_SIZE];

	memset(path,0,MAX_PATH);

	/* recv filepath */
	if(0 >= (r_cnt = read(readfd,path,MAX_PATH))){
		printf("[Server] error: you never give the filepath!\n");
		return (-1);
	}
	printf("[Server] read file: %s\n",path);
	/* open file */
	if( 0 > (fd = open(path,O_RDONLY))) {
		perror("[Server] open");
		exit (-1);
	}

	/* read file and sent file content */
	memset(buf,0,MAX_BUF_SIZE);
	while( 0 < (r_cnt = read(fd,buf,MAX_BUF_SIZE))) {
		write(writefd,buf,r_cnt);
		memset(buf,0,MAX_BUF_SIZE);
	}
	close(fd);
	return 0;
}

#endif