#include "FileServer.h"

using namespace std;



#define SERVER_BANNER \
"###############################\n"\
"#                             #\n"\
"#   Welcome to FileServer     #\n"\
"#                             #\n"\
"#      << command >>          #\n"\
"#    cd   :  change dir       #\n"\
"#    pwd  :  current dir      #\n"\
"#    ls   :  list files       #\n"\
"#    get  :  down a file      #\n"\
"#    exit :  logout           #\n"\
"#                             #\n"\
"###############################\n"

#define TIMEOUT      2

#define DEFAULT_PATH    "/tmp/"
#define BUF_SIZE        1024
#define MAX_FILE_NAME   256

typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr     SOCKADDR;


int sockfd;
int connfd;
socklen_t len;
ssize_t w_cnt;
ssize_t r_cnt;
SOCKADDR_IN serverAddr = {sizeof(SOCKADDR_IN)};
SOCKADDR_IN cliAddr = {sizeof(SOCKADDR_IN)};
int isReuseAddr = 1;

char command[BUF_SIZE];
char args[BUF_SIZE];
char buf[BUF_SIZE];



void showBanner(int clientfd) {

    if( 0 >= write(clientfd, SERVER_BANNER, sizeof(SERVER_BANNER))) {
        cerr<<"fail to send banner!\n";
    }
}



/* file filter */
int  dirfilter(const struct dirent *dt) {

    struct stat st;
    stat(dt->d_name, &st);
    if( st.st_mode & S_IFDIR) { //exclude dir

        return 0;
    }   
    return 1;
}

/* get all files in dir*/
int getFilesByPath(int connfd, char * path) {
        
    struct dirent **namelist;
    int n;

    if( 0> (n = scandir(path, &namelist, dirfilter, alphasort))) {
        cerr<<"scandir :"<<path<<endl;
        return -1; 
    }   
    if(namelist) {
        while(n--) {
            write(connfd, namelist[n]->d_name, strlen(namelist[n]->d_name));
            write(connfd, "\n", 1);
            cout<<namelist[n]->d_name<<endl;
            free(namelist[n]);
        }   
        write(connfd, "---------------------------------------\n", 40);
        free(namelist);
    }
    return 0;
}

int parseCommand(int clientfd, char* cmd, char* args) {
    char *p = cmd;
    int cmd_len = 0;
    int args_len = 0;

    if( 0 >= (r_cnt = read(clientfd, cmd, BUF_SIZE))) {
        cerr<<"client abort connection!\n";
        exit(0);
        return -1;
    }
    if(cmd[r_cnt-1] == '\n') {
        cmd[r_cnt-1] = '\0';
    }
    
    for( ; *p != ' ' && *p != '\0' ; p++ && cmd_len++ );
    
    if ('\0' == *p) {
        return 0;
    }
    *p = '\0';
    args_len = r_cnt - cmd_len;
    if( args_len > 0) {
        strncpy( args, (char*)(p+1), args_len);
    }
    //cout<<"cmd: "<<cmd<<"   args: "<<args<<endl;
    return 0;
}

/*
 *check file type
 *
 *  1:  dir
 *  2:  file
 * -1:  error
 */
int checkfiletype(char* file) {

    struct stat st;

        /* check dir*/
        if(-1 == stat(file, &st)) {
            cerr<<"file or dir not found!"<<endl;
            return -1;
        }
        if(st.st_mode & S_IFDIR) {
            cout<<"it's a dir"<<endl;
              return 1;
        }else {
            cout << "it's a file"<<endl;
            return 2;
       }
}

int do_changedir(int connfd, char* dir) {

    if( NULL != dir && !strlen(dir) ) {
        strcpy(dir, DEFAULT_PATH);
    }else {
        if(1 != checkfiletype(dir)) {
            write(connfd, "invaild dir\n",12);
            return -1;
        }
    }
    /* change dir*/
    chdir(dir);
    memset(buf, 0, BUF_SIZE);
    snprintf(buf,BUF_SIZE,"chdir to %s\n",dir);
    write(connfd,buf,strlen(buf));

    return 0;
}

int do_pwd(int connfd) {
    
    memset(buf, 0, BUF_SIZE);
    /* get current dir path */
    getcwd(buf,BUF_SIZE);
    write(connfd, buf, sizeof(buf));
    write(connfd, "\n",1);
    cout<<"pwd: "<<buf<<endl;

}

int do_listfiles(int connfd) {
    
    /* output files in dir*/
    getFilesByPath(connfd,".");

    return 0;
}


int do_downfile(int connfd, char* filename) {

    int fd;
    struct stat st;
    size_t len;
    ssize_t sum;

    /* validate filename*/
    if(2 != checkfiletype(filename)) {
        write(connfd,"file not found!\n",16);
        return -1;
    }
    if( -1 == (fd = open(filename,O_RDONLY ))) {
        cerr<<"open "<<filename<<endl;
        return -1;
    }
    memset(buf, 0, BUF_SIZE);
    stat(filename, &st);
    len = st.st_size;
    snprintf(buf, BUF_SIZE, "%d", len);
    cout<<"filename: "<<filename<<"  size: "<<buf<<"B"<<endl;
    /* send filesize*/
    write(connfd,buf,strlen(buf));
    sleep(TIMEOUT);
    /* begin to send a file */
    sum = 0;
    while(1) {
        memset(buf, 0 ,BUF_SIZE);
        r_cnt = read(fd, buf, BUF_SIZE);
        if(0 >= r_cnt) {
            break;
        }
        w_cnt = write(connfd, buf, r_cnt);
        sum += w_cnt;
        //cout<<"sum:"<<sum<<endl;
        if( 0 >= w_cnt) {
            /*client stop read!*/
            break;
        }

    }
    
	if(sum == len) {
		cout << "send completely!"<<endl;
	}else {
		cout << "lost "<<len -sum<<"bytes!"<<endl;
	}

    return 0; 
}


void do_logout(int connfd) {
    write(connfd, "bye!\n", 5);
    close(connfd);
    exit(0);
}


int service_dispatch(int clientfd, char *cmd, char *args) {

    int error = 0;

    if(!cmd) {
        error = 1;
    }
    cout<<"cmd:" <<cmd<<endl;
    cout<<"args: "<<args<<endl;
    if(!strcasecmp(cmd, "CD")) {  //change dir
        do_changedir(connfd, args);
    } 
    else if(!strcasecmp(cmd, "PWD")) {  //print current dir
        do_pwd(connfd);
    } 
    else if(!strcasecmp(cmd, "LS")) {  //list all files
        do_listfiles(connfd);
    } 
    else if( !strcasecmp(cmd,"GET")) {    //send an file to client
        do_downfile(connfd, args);
    }
    else if( !strcasecmp(cmd,"EXIT")) {    //logout
        do_logout(connfd);
    }else {
        error = 1;
    }

    if(error) {
        write(connfd, "invaild command\n",16);
        cerr<<"invaild command: "<<cmd<<endl;
        return -1;
    }

    return 0;
}


int startProcess(int connfd, struct sockaddr_in* cliAddr) {


       /* print client info */
    cout << "accept client : "<< inet_ntoa(cliAddr->sin_addr) << ":"\
                <<ntohs(cliAddr->sin_port)<<endl;
                
    chdir(DEFAULT_PATH);
    /*show banner to client*/
    showBanner(connfd);
    
    while(1) {
        /* reset buffer */
        memset(command, 0, BUF_SIZE);
        memset(args, 0, BUF_SIZE);
        /* recv command and parse it */
        if(-1 == parseCommand(connfd, command, args)) {
            /* client abort connection */
            close(connfd);
            break;
        }
        /* dispatch command */
        service_dispatch(connfd, command, args);
    }

}

/* start fileserver*/
int startFileServer(const char *ip, int port ) {
       
    char *cmd = NULL;
    pid_t pid;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    
    if( -1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        cerr<<"socket!\n";
        exit(-1);
    }
    
    /* reuse port */
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &isReuseAddr, sizeof(isReuseAddr)); 

    if( -1 == bind(sockfd, (SOCKADDR*) &serverAddr, sizeof(serverAddr))) {
        cerr<<"bind!\n";
        exit(-1);
    }

    if( -1 == listen(sockfd, 0)) {
        cerr<<"listen!\n";
        exit(-1);
    }

    cout<<"fileserver has listened at " << ip << ":"<<port<<endl;
    
    while(1) {
    
        len = sizeof(cliAddr);
        if (-1 == (connfd = accept(sockfd, (SOCKADDR*) &cliAddr, &len))) {
            cerr<"accept!\n";
            continue;
        }
    
        if( 0> (pid = fork())) {
        
            cerr<<"fork error!"<<endl;
            continue;
        }
        
        if( 0 == pid) { //child process
             
            startProcess(connfd, &cliAddr) ;
       
        }

        close(connfd);

    }
    return 1;
}


//:end
