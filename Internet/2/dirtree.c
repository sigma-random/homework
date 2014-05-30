#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>

#define MAX_DIR_DEPTH   256

typedef unsigned char byte;

byte map[MAX_DIR_DEPTH] = {0};
        
int selectdir(const struct dirent*);
char * make_format(int);
int dirtree(char * , int*);

int main(int argc, char **argv)
{
    int count = 0;
    int depth = 0;
    char filetype[10] = {0};
    if(argc != 2)
    {
        printf("usage: %s dirname\n", argv[0]);
        exit(-1);
    }
    count = dirtree(argv[1], &depth);
    printf("\n\ntotal %d files\n", count);
    return 0;
}


//#########################################################//

/* print files recursively */
int dirtree(char *dir, int *depth )
{
    struct dirent **pDirEntList;
    struct stat st;
    int total = 0;
    int index = 0;
    int count = 0;
    byte isFinalFile;
    char *filename;
    char *format;

    /* change dir*/
    total = scandir(dir, &pDirEntList, selectdir, alphasort);
    if(!total)
    {
        return 0;
    }
    if(-1 == chdir(dir))
    {
        printf("error: chdir %s\n",dir);
        exit(-1);
    }
    while(index < total)
    {
        /* make format string*/
        format =  make_format(*depth);
        filename = pDirEntList[index]->d_name;
        printf("%s%s\n", format, filename);
        free(format);       
         
        if(-1 == lstat(filename, &st))
        {
            perror("lstat");
            exit(-1);
        }
        
        if( S_IFDIR == (st.st_mode & S_IFMT) )
        {
             isFinalFile = (index == (total - 1)) ? 1 : 0;
             map[*depth] = isFinalFile;
            (*depth)++;
            count += dirtree(filename, depth);
            (*depth)--;
            map[*depth] =  0;
        }
        index++;
        count ++;
    }
    /* return to top dir*/
    if(-1 == chdir(".."))
    {
        perror("chdir");
        exit(-1);
    }
    return count;
}

/* make format string */
char * make_format(int depth)
{
    int len = 0;
    int len1 = 0;
    int len2 = 0;
    int i = 0;
    char tmplate1[] = "|   ";
    char tmplate2[] = "|-- ";
    char *format;
    len1 = depth * strlen(tmplate1);
    len2 = strlen(tmplate2) ;
    len = len1 + len2;
    format = malloc( len + 1) ;
    strcpy(format, "");
    for(i = 0; i < depth; i++)
    {
        strcat(format, tmplate1);
    }        
    strcat(format, tmplate2);
    for(i = 0; i< depth; i++)
    {
             if(map[i])
             {
                format[ strlen(tmplate1) * (i) ] = ' ';
             }
    }
    format[len] = 0;
    return format;
}

/* filter  dir */
int selectdir(const struct dirent* pDirEnt)
{
    /* ignore two special dirs :  "." and ".." */
    if(!strcmp(pDirEnt->d_name, ".")  || \
       !strcmp(pDirEnt->d_name, "..") )
    {
        return 0;
    }
    return 1;
}


