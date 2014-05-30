#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>


typedef unsigned int uint;

int selectdir(const struct dirent* );
void getfiletype(struct stat* , char *);
void getfilemode(struct stat* , char *);
char* getlinkfile(const struct stat *, char *);
void getfilelist(char *);

char *format = "%s%9s %2d %-10s %-10s %9d %-15s %-s\n";
char *parentdir = "..";
char mode[10] = "---------";    /*file flags*/
char filetype[2] = "-";         /*file type*/


int main(int argc, char **argv)
{
 
    if(argc != 2)
    {
        printf("usage: %s  dirname\n",argv[0]);
        exit(-1);
    }
    getfilelist(argv[1]);
    return 0;
}

//###############################################//

void getfilelist(char *dir)
{
    struct dirent **pDirEntList;
    struct stat st;
    struct passwd *usrinfo;
    struct group *grpinfo;
    char *filename, *buf;
    char *mtime;
    int total = 0;
    int index = 0;
    int nlinknum;    
    int blocks;
    uint size;

    total = scandir(dir,&pDirEntList,selectdir,alphasort);
    if(!total)
    {
        printf("error: open dir %s\n",dir);
        exit(-1);
    }
    if(-1 == chdir(dir))    /* change current directory to  call stat */
    {
        printf("error: chdir -> %s\n",dir);
        exit(-1);
    } 
    /* get blocks*/
    blocks = 0;
    do
    { 
        filename = pDirEntList[index]->d_name;
        if( -1 == lstat(filename,&st))
        {
            printf("error: lstat(%s)\n",filename);
            exit(-1);
        }
        blocks += st.st_blocks;
    }while((++index)<total);
    blocks = blocks/2;
    printf("total: %d\n",blocks);
    
    /*get file list*/
    index = 0;
    do
    { 
        filename = pDirEntList[index]->d_name;

        if( -1 == lstat(filename,&st))
        {
            printf("error: lstat(%s)\n",filename);
            exit(-1);
        }
        mtime = ctime(&(st.st_mtime));
        mtime += 4;
        mtime[strlen(mtime)-9] = 0;
        size = st.st_size;
        nlinknum = st.st_nlink;
        usrinfo = getpwuid(st.st_uid);
        grpinfo = getgrgid(st.st_gid);
        if(S_ISLNK(st.st_mode))
        {
            buf = getlinkfile(&st,filename);
            filename = buf;
        }
        getfiletype(&st, filetype);
        getfilemode(&st, mode);
        printf(format,filetype,\
                        mode,\
                        nlinknum,\
                        usrinfo->pw_name,\
                        grpinfo->gr_name,\
                        size,\
                        mtime,\
                        filename);
  
    }while((++index) < total);
 
     if(-1 == chdir(parentdir))    /* change current directory to  top level */
    {
        printf("error: chdir -> %s\n",parentdir);
        exit(-1);
    }  
}



void getfiletype(struct stat* st, char *filetype)
{
    char t;
    switch( st->st_mode & S_IFMT )
    {
        case S_IFDIR:
                t = 'd';
                break;
        case S_IFREG:
                t = '-';
                break;
        case S_IFLNK:
                t = 'l';
                break;
        case S_IFBLK:
                t = 'b';
                break;
        case S_IFIFO:
                t = 'f';
                break;
        case S_IFSOCK:
                t = 's';
                break;
        case S_IFCHR:
                t = 'c';
                break;
        default:
                t = '?';
                break;
    }
    filetype[0] = t;
}


void getfilemode(struct stat* st, char *mode)
{
    int m = st->st_mode & 0x7FF;
    if(m & S_IRUSR)
    {
        mode[0] = 'r';
    }
    if(m & S_IWUSR)
    {
        mode[1] = 'w';
    }
    if(m & S_IXUSR)
    {
        mode[2] = 'x';
    }
    if(m & S_IRGRP)
    {
        mode[3] = 'r';
    }
    if(m & S_IWGRP)
    {
        mode[4] = 'w';
    }
    if(m & S_IXGRP)
    {
        mode[5] = 'x';
    }
    if(m & S_IROTH)
    {
        mode[6] = 'r';
    }
    if(m & S_IWOTH)
    {
        mode[7] = 'w';
    }
    if(m & S_IXOTH)
    {
        mode[8] = 'x';
    }
    if(m & S_ISUID)
    {
        mode[2] = 's';
    }
    if(m & S_ISGID)
    {
        mode[5] = 's';
    }
}




char* getlinkfile(const struct stat *st,char * linkfilename)
{
    int len;
    char * realfilename, *buf;
    char * mark = " -> ";
    
    len = st->st_size;
    realfilename = malloc(len + 1);
    if(realfilename == NULL)
    {
        printf("error: mallco \n");
        exit(-1);
    }
    if(0 > readlink(linkfilename, realfilename, len + 1))
    {
        printf("error: readlink %s\n",linkfilename);
        exit(-1);
    }
    realfilename[len] = 0;
    len = strlen(linkfilename) + strlen(realfilename) + strlen(mark);
    buf = malloc(len + 1);
    if(buf == NULL)
    {
        printf("error: mallco \n");
        exit(-1);
    }
    snprintf(buf, len+1, "%s%s%s", linkfilename, mark, realfilename);
    buf[len] = 0;
    free(realfilename);
    return buf;
}

int selectdir(const struct dirent* pDirEnt)
{
    /* ignore two special dirs :  "." and ".." */
    if(!strcmp(pDirEnt->d_name, ".") ||\
       !strcmp(pDirEnt->d_name, "..") )
    {
        return 0;
    }
    return 1;
}

