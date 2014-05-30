
#ifndef _INIFILE_H_
#define _INIFILE_H_
#include "Common.h"


#define MAX_BUFSIZE 1024

class INIParser
{
public:
	static char * rstrip(char *word);
    static char *GetInitKey(const char *filename, const char *title, const char *key);
};



#endif

 /*
 * File: inifile.h
 * Read INI File
 */

 /*--------------*
 * Test Case: *
 *--------------*
 config.ini

 ;[test]
#p2= 789
;p2= 9
p2 = 456


[test]
p3 = p2
pp2= 45
p2 = "456"


[ip]
portexam=777
[user]
port=666
 *--------------*/