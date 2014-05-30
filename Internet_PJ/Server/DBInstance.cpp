#include "include/DBInstance.h"


DBInstance::DBInstance(const char* charset)
{
	m_MysqlConn = NULL;
	m_Sql = NULL;
	m_Charset = charset;
	getDBConn();
	//Logger("DBInstance\n");
}

DBInstance::~DBInstance()
{
	closeDB();
	Logger("Call ~DBInstance()\n");
}

void DBInstance::getDBConn()
{
	    MYSQL* mysqlConn = new MYSQL();
	    assert(mysqlConn != NULL );
	    if(NULL== mysql_init(m_MysqlConn))
	    {
	    	lastMysqlError();
	    	return;
	    }
	    m_MysqlConn = mysqlConn;
	    Logger("mysql_init success!\n");
}


void DBInstance::connectDB(const char *host, const char *user, const char *passwd, 
							const char *db, unsigned int port, const char *unix_socket,
							unsigned long client_flag)
{
	MYSQL * connectHandle;
	char *charset;
	assert(m_MysqlConn != NULL);
    if(NULL != m_MysqlConn)
    {
	    if(NULL== (connectHandle = mysql_real_connect(	m_MysqlConn,
												    	host,
												    	user,
												    	passwd,
												    	db,
												    	port,
												    	unix_socket, 
												    	client_flag)))
	    {
	    	lastMysqlError();
	    	exit(0);
	    	return;
	    }
	    Logger("connect mysql database success!\n");
	    Logger("mysql  server   =>  %s:%d\n",host,port);
	    Logger("mysql  user     =>  \"%s\"\n",user);
	    Logger("mysql  pass     =>  \"%s\"\n",passwd);
	    Logger("mysql  database =>  \"%s\"\n",db);
	    /* set charset*/
		char charset[50]={0};
		int len = strlen(m_Charset)>40 ? 40 : strlen(m_Charset);
		assert(m_Charset != NULL);
		assert(m_MysqlConn != NULL);
		strcpy(charset,"set names ");
		strncat(charset,m_Charset,len);
		Logger("mysql  charset  =>  \"%s\"\n",charset);
	    mysql_query(m_MysqlConn, charset); 
	    lastMysqlError();
	}
}


void DBInstance::setCharset(const char* charset)
{
	m_Charset = charset;
} 

void DBInstance::doSafeQuery(char* sql)
{
	MYSQL_RES *result; 		//指向查询结果的指针 
	MYSQL_FIELD *field; 	//字段结构指针
	MYSQL_ROW result_row; 	//按行返回的查询信息
	int row, column; 		//查询返回的行数和列数
	int i,j;

	assert(sql != NULL);
	assert(m_MysqlConn != NULL);
	m_Sql = sql;
	//mysql_real_escape_string(m_MysqlConn, m_Sql, sql, strlen(sql));
	Logger("sql:%s\n",m_Sql);
	if( 0 != mysql_real_query(m_MysqlConn,m_Sql,strlen(m_Sql)))
	{
		this->lastMysqlError();
		return;
	}
	//Logger("execute sql success!\n");
	result = mysql_store_result(m_MysqlConn);
	if (result) {  
		column = mysql_num_fields(result); 
		row = mysql_num_rows(result) ; 
		Logger("total rows: %d\n",row);
	    // 输出結果的字段名 
		//for (i = 0; field = mysql_fetch_field(result); i++) 
		//{
		//	cout<<field->name<<endl; 
		//}
		
		//按行输出結果
		cout<<"------------------------------------------------------------------------------"<<endl;
		for (i = 1; i < row; i++) 
		{ 
			result_row = mysql_fetch_row(result); 
			for (j = 0; j < column; j++) 
			{
				cout<<result_row[j]<<" ";
			}
			cout<<endl;
		} 
		cout<<"------------------------------------------------------------------------------"<<endl;
		mysql_free_result(result); // 释放结果集
	}
}


void DBInstance::closeDB( )
{
    if(NULL != m_MysqlConn)
    {
	    mysql_close(m_MysqlConn);
	    lastMysqlError();
	    //Logger("mysql_close success!\n");
	}
}

void DBInstance::lastMysqlError()
{
	assert(m_MysqlConn != NULL);
	if(0 == strlen(mysql_error(m_MysqlConn)))
	{
		return ;
	}
	Logger("error: %s\n",mysql_error(m_MysqlConn));
	return ;
}

