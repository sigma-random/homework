#ifndef __DB_INSTANCE__
#define __DB_INSTANCE__

#include "Common.h"
#include "Logger.h"
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>


class DBInstance
{
public:
	DBInstance(const char* charset="utf8");
	virtual ~DBInstance();
	virtual void getDBConn();
	virtual void connectDB(const char *host, const char *user, const char *passwd, const char *db, 
		unsigned int port, const char *unix_socket = NULL,unsigned long client_flag = CLIENT_FOUND_ROWS);
	virtual void setCharset(const char* _charset);
	virtual void doSafeQuery(char* sql);
	void sqlBadCharsFilter();		/* avoid sql injection */
	virtual void lastMysqlError();
	virtual void closeDB();

public:
	MYSQL* m_MysqlConn;
	DBInstance* m_Instance;
	const char *m_Charset;
	char *m_Sql;
};

#endif