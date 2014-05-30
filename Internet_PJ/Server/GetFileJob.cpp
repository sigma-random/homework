#include "include/GetFileJob.h"	
#include "include/Logger.h"	

char*  GetFileJob::m_Dir       = NULL;
char*  GetFileJob::m_IndexFile = NULL;

GetFileJob::GetFileJob(char *dir,char *indexfile)
{
	assert(dir != NULL );
	m_Dir = dir;
	assert(indexfile != NULL );
	m_IndexFile = indexfile;
	checkFile(m_IndexFile);
	Logger("GetFileJob() data_dir: %s\n",dir);
	Logger("GetFileJob() index_file: %s\n",m_IndexFile);
}

GetFileJob::~GetFileJob(){ }

void   
GetFileJob::setFileDir(char *dir) {m_Dir = dir;}

char*  
GetFileJob::getFileDir(char *dir) {return m_Dir;}


char * 
GetFileJob::buildRealPath(char *dir, char *file)
{
	char* realpath = NULL;
	assert(dir != NULL);
	assert(file != NULL);
	int len = strlen(dir);
	realpath = (char*)malloc( len + strlen(file) + 1);
	strcpy(realpath,dir);
	strcat(realpath,file);
	Logger("realpath:%s\n",realpath);
	return realpath;
}

bool   
GetFileJob::checkFile(char *filepath)
{
	if((access(filepath,F_OK))!=0)
	{
		cout<<"[ERROR] file: "<<filepath<<" is not exsit!"<<endl;
		return false;
	}
	return true;
}

void
GetFileJob::read_to_fd(char *file,int _fd)
{
	int rd_count = 0;
	int fd;
	string result;
	char buf[1024]={0};
	if(checkFile(file)==false){write(_fd,"file not exist!\n",16);}
    if(0 > (fd = open(file, O_RDONLY)))
    {
        perror("can't open file\n");
        write(_fd,"request error\n",14);
        return;
        //exit(-1);
    }
    do
    {
        if(-1 == (rd_count = read(fd, buf, 1024)))
        {
            perror("read\n");
            break;
        }
        write(_fd,buf,rd_count);
        memset(buf,0,1024);
    }while(rd_count>0);
    close(fd);
}

char*  
GetFileJob::getFileContent(char *file){ }

void   
GetFileJob::getFileContent( char *file, int fd)
{
	char *realpath = buildRealPath(m_Dir,file);
	checkFile(realpath);
	GetFileJob::read_to_fd(realpath,fd);
	free(realpath);

}


void 
GetFileJob::GetValueByTagAndOffset(char * tag,long index,int fd)
{
	ifstream xmlInfile;
	vector<string> items;
	vector<string> new_items;
	assert(m_IndexFile!=NULL);
	xmlInfile.open(m_IndexFile,ifstream::in);
	xmlInfile.seekg(index);
	items=GetFileJob::GetValuesByTag(tag,xmlInfile);
	vector<string>::iterator i_vec=items.begin();
	int pager = 20;
	for(;i_vec!=items.end();++i_vec,pager--){
		if(pager == 0)
			break;
		cout<<*i_vec<<endl;
		//write(fd,i_vec->c_str(),i_vec->size());
		//write(fd,"\n",1);
		char * file = strstr((char*)i_vec->c_str(),"|");
		*file = 0;
		file = GetFileJob::buildRealPath(GetFileJob::m_Dir,(char*)i_vec->c_str());
		GetFileJob::read_to_fd(file,fd);
	}
	xmlInfile.close();
}





void   
GetFileJob::RunJob(void *job_param)
{
	assert(job_param != NULL );
	RequestData *ReqData = (RequestData*)job_param;
	char* file = ReqData->data;
	assert(file!=NULL);
	int fd  = ReqData->fd;
	Logger("GetFileJob::RunJob() file:%s\n",file);
	getFileContent(file, fd);	
	//shutdown client socket
	//############################
	shutdown(fd,SHUT_RDWR);						
	//############################
	Logger("shutdown the client socket\n");

}
