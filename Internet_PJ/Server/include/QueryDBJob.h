#ifndef __QUERY_DB_JOB__
#define __QUERY_DB_JOB__

#include "Common.h"
#include "GetFileJob.h"
#include "../ThreadPool/include/Job.h"
#include "DBInstance.h"

class GetFileJob;

// implement DBInstance class And Job Interface
class QueryDBJob: public DBInstance , public Job
{

public:
	QueryDBJob(const char *host, unsigned int port, const char *user, const char *passwd, const char *db, 
		const char* charset="utf8",const char *unix_socket = NULL,unsigned long client_flag = CLIENT_FOUND_ROWS);
	virtual ~QueryDBJob();
	void do_query_keyword(const char* keyword,int fd);		// redirect output to fd
	void doQuerySQL(char* sql,int fd);							// redirect output to fd
	void doQuerySQL(char* sql);
	void RunJob(void *job_param);
};
#endif