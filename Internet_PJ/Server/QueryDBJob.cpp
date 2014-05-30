
#include "include/QueryDBJob.h"


QueryDBJob::QueryDBJob(const char *host, unsigned int port, const char *user, const char *passwd, const char *db, 
		const char* charset,const char *unix_socket,unsigned long client_flag)
{
	this->getDBConn();
	
	this->setCharset(charset);	/* set database charset from config file*/
	
	this->connectDB(host,user,passwd,db,port);
}

QueryDBJob::~QueryDBJob()
{
	Logger("Call ~QueryDBJob()\n");
}


void 
QueryDBJob::doQuerySQL(char* sql)
{

}

void 
QueryDBJob::doQuerySQL(char* sql,int fd)
{
	MYSQL_RES *result; 		//指向查询结果的指针 
	//MYSQL_FIELD *field; 	//字段结构指针
	MYSQL_ROW result_row; 	//按行返回的查询信息 
	int row, column; 		//查询返回的行数和列数
	int i,j;

	assert(sql != NULL);
	m_Sql = sql;
	assert(m_MysqlConn != NULL);
	if( 0 != mysql_real_query(m_MysqlConn,m_Sql,strlen(m_Sql)))
	{
		lastMysqlError();
		return ;
	}
	result = mysql_store_result(m_MysqlConn);
	if (result) {  
		column = mysql_num_fields(result); 
		row = mysql_num_rows(result); 
		Logger("total rows: %d\n",row);
		if( 0 == row )
		{
			return;
		}
	    //输出結果的字段名
		//for (i = 0; field = mysql_fetch_field(result); i++) 
		//{
		//	cout<<field->name<<endl; 
		//}
		// 按行输出結果
		// for (i = 0; i < row; i++) 
		// { 
		// 	result_row = mysql_fetch_row(result); 			
		// 	for (j = 0; j < column; j++) 
		// 	{
		// 		write(fd,result_row[j],strlen(result_row[j]));
		// 		write(fd,"|",1);
		// 		cout<<result_row[j]<<"|";
		// 	}
		// 	write(fd,"\n",1);
		// 	cout<<endl;
		// } 

		//for debug
		assert(row<=1);
		for (i = 0; i < row; i++) 
		{ 
			result_row = mysql_fetch_row(result); 			
			write(fd,result_row[2],strlen(result_row[2]));
			write(fd,"\n\n",1);
			cout<<result_row[2]<<endl;
			GetFileJob::GetValueByTagAndOffset("index",atol(result_row[2]),fd);
			break;
		} 
		mysql_free_result(result); // 释放结果集
	}
}


void 
QueryDBJob::do_query_keyword(const char* keyword, int fd)
{
	char * sql;
	const char *baseSQL = "select * from user where user=";
	int len = strlen(baseSQL)+strlen(keyword)+2+1;
	assert(keyword != NULL );
	sql = (char*)malloc(len+1);
	assert(sql != NULL );
	snprintf(sql,len,"%s'%s'",baseSQL,keyword);		//dangerous	, SQL injection
	sql[len] = '\x00';
	Logger("sql=%s\n",sql);
	doQuerySQL(sql,fd);
	//shutdown client socket
	//############################
	shutdown(fd,SHUT_RDWR);						
	//############################
	Logger("shutdown the client socket\n");
	free(sql);
}

//implement Job::RunJob interface
void 
QueryDBJob::RunJob(void *job_param)
{
	assert(job_param != NULL );
	RequestData *ReqData = (RequestData*)job_param;
	char* sql = ReqData->data;
	assert(sql!=NULL);
	int fd  = ReqData->fd;
	Logger("doQuerySQL: %s\n",sql);
	doQuerySQL(sql, fd);	
	//shutdown client socket
	//############################
	shutdown(fd,SHUT_RDWR);						
	//############################
	Logger("shutdown the client socket\n");
}
