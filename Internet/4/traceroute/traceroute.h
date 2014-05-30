#ifndef	__TARCEROUT__
#define	 __TARCEROUT__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <setjmp.h>

#define BUFSIZE 1500

int trace_info(int ,  char **);
int package_paser(char *);

#endif