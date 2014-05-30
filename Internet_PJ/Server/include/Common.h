#ifndef __COMMON__
#define __COMMON__



#include <unistd.h>
#include <sys/types.h>

#include <errno.h>
#include <assert.h>
#include <stdarg.h>

#include <signal.h>
#include <sys/wait.h>

#include <time.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/fcntl.h>
#include <pthread.h>

#include <wchar.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <pthread.h>
#include <cstdlib>
#include <cassert>
#include <unistd.h>
#include <exception>
#include <string>

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <map>
using namespace std;


typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;


typedef struct 
{
	char *data;
	int   fd;
}RequestData;


#endif

