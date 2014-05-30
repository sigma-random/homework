#include "include/Logger.h"


void Logger(const char *fmt , ...)
{

	char *config_file = Config::getConfigFile();
	assert(config_file!=NULL);
	if(0 == strcmp("on" ,Config::getSectionItem(config_file,"debug","isdebug")))
	{
		/*
		time_t ti;
		ti = time(NULL);
		char times[65535];
		snprintf(times , sizeof(times) , "%s" , ctime(&ti));
		times[strlen(times)-1] = ' ';
		strncat(times , "-----------    " , 9);
		*/
		va_list ap;
		char buff[65535];
		va_start(ap , fmt);
		vsnprintf(buff , sizeof(buff) ,  fmt , ap);	
		if (0 == strlen(Config::getSectionItem(config_file,"debug","title")))
		{
			cout<<"[DEBUG] "<<buff;
		}
		else
		{
			cout<<"["<< Config::getSectionItem(config_file,"debug","title") <<"] "<<buff;	
		}
		//strncat(times , buff , strlen(buff));
		//fwrite(times , sizeof(char) , strlen(times) , pt);
		//fwrite("\n" , 1 , 1 , pt);
		//fflush(pt);
		va_end(ap);
	}

}


//######################################################################################//
//#define LOGGER_TEST
#ifdef  LOGGER_TEST
#define MAKE "g++ Logger.cpp  Config.cpp INIParser.cpp -o Logger"

int main(int argc, char** argv)
{
	Logger("sssssssssssss\n");
    return 0;
}
#endif