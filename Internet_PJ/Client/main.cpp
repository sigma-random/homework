#include "include/Common.h"
#include "include/Config.h"
#include "include/Logger.h"
#include "include/TcpClient.h"
#include "include/RequestPaperService.h"
#include "include/SearchWordService.h"

//####################################################################################

#define INT_CAST(num)			atoi(num)
#define CLIENT_CONF_FILE		"conf/client.properties"
#define CONFIG(section,item)	Config::getSectionItem(CLIENT_CONF_FILE,section,item)
#define SERVER_IP				CONFIG("server","ip")
#define SERVER_PORT				CONFIG("server","port")

//####################################################################################

void startClient(void)			__attribute__((constructor));
void shutDown(void)				__attribute__((destructor));
void sig_handler(int signum);

//####################################################################################
TcpClient *client;


//####################################################################################

int		
main(int argc, char** argv)
{
	return 0;
}

void 
startClient()
{
	char* data=NULL;
	signal(SIGINT,sig_handler);
	new Config(CLIENT_CONF_FILE);
	client = new TcpClient(SERVER_IP, INT_CAST(SERVER_PORT));	
	data = (char*)client->queryDataOnce("keyword=root");
	cout<<"data:\n"<<data<<endl;
}

void
reaseResource()
{
	if(client) {delete client;client=NULL;}
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