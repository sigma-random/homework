#ifndef __GET_FILE_JOB__
#define __GET_FILE_JOB__

#include "Common.h"

#include "XMLParser.h"
#include "../ThreadPool/include/Job.h"

// implement XMLParser class And Job Interface
class GetFileJob:public XMLParser ,public Job
{

public:
	GetFileJob(char *dir, char *indexfile);
	virtual ~GetFileJob();
	static char   *buildRealPath(char *dir, char *file);
	static bool   checkFile(char *filepath);
	char   *getFileContent(char *file);
	void   getFileContent(char *file, int fd = STDOUT_FILENO);		// output file content to fd
	void   setFileDir(char *dir);
	char   *getFileDir(char *dir);
	static void GetValueByTagAndOffset(char * tag,long index,int fd = STDOUT_FILENO);
	static void read_to_fd(char *file,int _fd = STDOUT_FILENO);
	void   RunJob(void *job_param);
private:
	static char  *m_Dir;
	static char  *m_IndexFile;
};
#endif