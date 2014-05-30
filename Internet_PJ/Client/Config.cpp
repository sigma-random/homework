#include "include/Config.h"


char* Config::m_ConfigFile = NULL;


Config::Config(const char* confile)
{
	this->setConfigFile(confile);
}

Config::~Config()
{

}

void Config::setConfigFile(const char* confile)
{
	assert(confile!=NULL);
	Config::checkConfigFile(confile);
	Config::m_ConfigFile = (char*)confile;
}

char* Config::getConfigFile()
{
	return Config::m_ConfigFile;
}

char* Config::getSectionItem(const char* confile, const char* section,const char* item)
{
	assert(confile != NULL);
	assert(section != NULL);
	assert(item != NULL);
	Config::checkConfigFile(confile);
	return INIParser::GetInitKey(confile,section,item);
}

void Config::checkConfigFile(const char* filepath)
{
	if((access(filepath,F_OK))!=0)
	{
		cout<<"[ERROR] file: "<<filepath<<" is not exsit!"<<endl;
		exit(-1);
	}

}


/* ################################################################################## */

//#define CONFIG_TEST
#ifdef  CONFIG_TEST
#define MAKE #define MAKE "g++  Config.cpp INIParser.cpp  -o config"

int main(int argc, char** argv)
{
    Config config =  Config(SERVER_CONFIG_FILE);		//"server.properties"
    cout<<Config::getSectionItem(SERVER_CONFIG_FILE,"debug","isdebug")<<endl;

    return 0;
}
#endif