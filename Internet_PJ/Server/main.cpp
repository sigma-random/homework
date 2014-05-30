#include "include/Common.h"
#include "include/Config.h"
#include "include/Logger.h"
#include "include/TcpServer.h"
#include "include/RequestHandler.h"
#include "include/QueryDBJob.h"
#include "include/GetFileJob.h"
#include "ThreadPool/include/ThreadPoolManage.h"

//####################################################################################

#define INT_CAST(num)			atoi(num)
#define SERVER_CONF_FILE		"conf/server.properties"
#define CONFIG(section,item)	Config::getSectionItem(SERVER_CONF_FILE,section,item)
#define SERVER_IP				CONFIG("server","ip")
#define SERVER_PORT				CONFIG("server","port")
#define SERVER_MAXCONN			CONFIG("server","maxconn")
#define DB_IP					CONFIG("database","ip")
#define DB_PORT					CONFIG("database","port")
#define DB_USER					CONFIG("database","user")
#define DB_PASS					CONFIG("database","pass")
#define DB_NAME					CONFIG("database","dbname")
#define DB_CHARSET				CONFIG("database","charset")
#define XMLDATA_DIR				CONFIG("data","XmlDir")				
#define INDEXFILE				CONFIG("data","IndexFile")				
#define TO_INITNUM				CONFIG("threadpool","initnum")

//####################################################################################

void startServer(void)			__attribute__((constructor));
void shutDown(void)				__attribute__((destructor));
void sig_handler(int signum);

//####################################################################################
TcpServer        *Server;
RequestHandler   *ReqHandler;
QueryDBJob       *QueryJob;
GetFileJob       *FileJob;
ThreadPoolManage *TPM;

//####################################################################################

int		
main(int argc, char** argv)
{

    return 0;
}


void 
startServer()
{

	signal(SIGINT,sig_handler);
	
	/* set server's config file */
	new Config(SERVER_CONF_FILE);

	/* set up thread pool */
	TPM        = new ThreadPoolManage(INT_CAST(TO_INITNUM));

	/* init database connection */
	QueryJob   = new QueryDBJob(DB_IP,INT_CAST(DB_PORT),DB_USER,DB_PASS,DB_NAME,DB_CHARSET);
	
	FileJob    = new GetFileJob(XMLDATA_DIR,INDEXFILE);

	/* set up tcp socket server */
	Server     = new TcpServer(SERVER_IP,INT_CAST(SERVER_PORT));
	Server->startSearchServer(INT_CAST(SERVER_MAXCONN));
	
	/* init Request Handler */
	ReqHandler = new RequestHandler();
	
	/* register Request Handler to TCP Socket Server */
	Server->register_handler(ReqHandler);

	/* bind thread pool whith request handler*/
	ReqHandler->BindThreadPool( TPM);

	/* bind QueryDBJob with Socket Request Handler */
	ReqHandler->BindDBServiceJob(QueryJob);

	/* bind GetFileJob with Socket Request Handler */
	ReqHandler->BindFileServiceJob(FileJob);
	
	/* Handle CLient's TCP Socket Request */
	ReqHandler->handleClient(ReqHandler->getServerfd());
}


void
reaseResource()
{
	if(Server)     {delete Server; Server = NULL;}
	if(ReqHandler) {delete ReqHandler; ReqHandler = NULL;}
	if(QueryJob)   {delete QueryJob;QueryJob = NULL;}
	if(TPM)        {delete TPM; TPM = NULL;}	
	Logger("release all resources!\n");
}

void 
shutDown(void)
{
	Logger("shutDown...\n");
	reaseResource();
}


void
sig_handler(int signum)
{
	switch(signum)
	{
		case SIGINT:
			cout<<endl<<endl;
			Logger("want to quit?\n");
			//reaseResource();
			break;
		default:
			break;
	}
}