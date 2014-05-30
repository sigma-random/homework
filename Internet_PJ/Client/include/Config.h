#ifndef __CONFIG__
#define __CONFIG__

#include "Common.h"
#include "INIParser.h"

class Config
{
public:
	Config(const char* confile);
	virtual ~Config();
	static void setConfigFile(const char* confile);
	static char* getConfigFile();
	static void checkConfigFile(const char* confile);	
	static char * getSectionItem(const char* m_ConfigFile, const char* section,const char* item);

private:
	static char* m_ConfigFile;
};

#endif
