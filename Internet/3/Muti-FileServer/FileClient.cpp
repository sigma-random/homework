
#include "FileClient.h"

using namespace std;



typedef struct sockaddr_in SOCKADDR;


#define BUF_SIZE        1024
#define MAX_FILE_NAME   256
int sockfd;
socklen_t len;
ssize_t r_cnt;
ssize_t w_cnt;
SOCKADDR cliAddr = {sizeof(SOCKADDR)};
SOCKADDR serverAddr = {sizeof(SOCKADDR)};

char command[BUF_SIZE];
char args[BUF_SIZE];
char buf[BUF_SIZE];




void showBanner(int sockfd) {

    memset(buf, 0 ,BUF_SIZE);
    r_cnt = read(sockfd, buf, BUF_SIZE);
    cout << buf << endl;
}

void processor(float percent, int finished) {

    int i = 0;
    cout<<std::left<<setw(3)<<percent<<std::left<<setfill(' ')<<setw(8)<<"%"<<"\r";
    if(finished) {
        cout <<endl;
    }
}




/* get command form stdout*/
int parseCommand(char *cmd) {
   
    char *p;
    int len = 0;
    int cmd_len = 0;
    int args_len = 0;
     
    cout<<"cmd:~$ ";
    cin.getline(cmd, BUF_SIZE);
    len = strlen(cmd);
    if(cmd[len-1] == '\n') {
         cmd[len-1] = '\0';
    }
    for( p = cmd; *p != ' ' && *p != '\0' ; p++ && cmd_len++ );
    if ('\0' == *p) {
        return 0;
    }
    *p = '\0';
    args_len = len - cmd_len - 1;
    if( args_len > 0) {
        strncpy( args, (char*)(p+1), args_len);
    }
    //cout<<"cmd: "<<cmd<<"   args: "<<args<<endl;
    return 0;
}


int do_downfile(int sockfd, char* cmd, char*args) {

    int len = strlen(cmd);
    char *filename;
    char newfilename[MAX_FILE_NAME];
    size_t filesize;
    size_t sum;
    int fd;


    /* send downfile commad*/
    memset(buf, 0, BUF_SIZE);   
    snprintf(buf, BUF_SIZE,"%s %s",cmd,args);
    write(sockfd, buf, strlen(buf));
    
    /* get filename and filesize */
    for(filename = args; *filename == ' '; filename++); 
    memset(buf, 0, BUF_SIZE);
    read(sockfd, buf, 30);
    filesize =(ssize_t)atoll(buf);
    memset(newfilename, 0 ,MAX_FILE_NAME);
    cout<<"[*] please input a new filename: ";
    cin.getline(newfilename,MAX_FILE_NAME); 
    /* save to file*/
    if(-1 == (fd =open(newfilename,O_CREAT|O_RDWR))){
        cerr<<"open file error"<<endl;
        return -1;
    }
    sum = 0;
    w_cnt = 0;
    memset(buf, 0, BUF_SIZE);
    while(  0 < (r_cnt = read(sockfd, buf, BUF_SIZE))){
       sum += r_cnt;
       w_cnt += write(fd, buf, r_cnt);
       memset(buf, 0, BUF_SIZE);
       processor(float(100.00*sum/filesize),0);
       if(sum == filesize){
            processor(float(100.00),1);
            cout<<"save file ["<<filename<< "] to ["<<newfilename<<"]   size: "<<sum<<" bytes!\n"<<endl;
            break;
       }
    }
    if(fd) {
        close(fd);
    }
    return 0;
}


int do_postCommand(int sockfd, char* cmd) {
    
    int len = strlen(cmd);
    write(sockfd, cmd, len);
    memset(buf, 0, BUF_SIZE);
    
    while( r_cnt = read(sockfd, buf, BUF_SIZE)){
      
        if(r_cnt < BUF_SIZE ){
            cout<<buf<<endl;
            break;
        }
        cout<<buf;
        memset(buf, 0, BUF_SIZE);
    }
    
    return 0;
}


int service_dispatch(int sockfd, char *cmd, char *args) {
    
    memset(buf, 0, BUF_SIZE);   
    snprintf(buf, BUF_SIZE,"%s %s",cmd,args);
    if(!strcasecmp(cmd,"GET")) {    //down a file from remote server
        do_downfile(sockfd, cmd, args);
    }else {
        do_postCommand(sockfd,buf);
    }
    return 0;
}

void connFileServer(char *ip, int port) {
    
    char *cmd;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
   
    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        cerr<<"socket error"<<endl;
        exit(-1);
    }

    len = sizeof(serverAddr);
    if( -1 ==  connect(sockfd,( struct sockaddr *)&serverAddr, len)){
        cerr<<"connect error"<<endl;
        exit(-1);
    }
    /* show server's banner */
    showBanner(sockfd);
    while(1) {
        memset(command, 0, BUF_SIZE);
        memset(args, 0, BUF_SIZE);
        parseCommand(command);
        service_dispatch(sockfd, command, args);
    }
}
