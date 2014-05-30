//client.cpp
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdio.h>

#include<algorithm>

#define SERVER_PORT 8080

void str_cli(char *data,int sockfd);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    //tcpcli <ipaddress> <data>
    if(argc!=3)
        return -1;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
       printf("socket established error: %s\n",(char*)strerror(errno)); 
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERVER_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    
    printf("client try to connect\n");
    int conRes=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(conRes==-1)
    {    
            perror("connect");
           printf("connect error: %s\n",strerror(errno));
    }

    str_cli(argv[2],sockfd);

    exit(0);
}

void str_cli(char *data,int sockfd)
{
    int n=0;
    char recv[512];

    FILE* fp=stdin;

    int maxfdp1;
    fd_set rset;
    char sendline[512],recvline[512];

    FD_ZERO(&rset);
    for(;;)
    {
        FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfdp1=std::max( fileno(fp),sockfd) +1;
        select(maxfdp1,&rset,NULL,NULL,NULL);            //使用select使得客户端不需要注释在标准IO 或者 TCP read的其中一个上

        if( FD_ISSET(sockfd,&rset))
        {
            if(read(sockfd,recvline,sizeof(recvline))==0) 
            //那边发来需要有换行符表示串尾，不然这边要求接受的字节数还不到，或者还没到尾部，所以没有输出
            { 
                printf("read error\n");
                //handle error；
                return;
            }
        //    printf("readed:%d,str:%d",cr,strlen(recvline));
        //      如果tcp缓冲区内有多于预定义取的字节，则会自动调用read再取，直到结尾或者遇到换行符
            fputs(recvline,stdout);                            //标准库函数fputs向标准IO上写一行
            bzero(&recvline,strlen(recvline));
        }
        if( FD_ISSET(fileno(fp),&rset))
        {
            if(fgets(sendline,sizeof(sendline),fp)==NULL)    //标准库函数fgets从标准IO上获取一行
            //fgets会加上换行符，如果有
                return;
            write(sockfd,sendline,strlen(sendline)-1);        
            //减一去除换行符
            bzero(&sendline,strlen(sendline));
        }
    }
    
}