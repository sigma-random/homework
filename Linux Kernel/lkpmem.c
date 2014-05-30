//lkpmem.c
#include <stdio.h>
#include <stdlib.h>

int globalvar1;					//in .bss 
int globalvar2 = 3;			//in .data


void 
mylocalfoo()
{
	int functionvar;				//in stack
	printf("variable functionvar \t location: 0x%x\n", &functionvar);
}


int 
main()
{
	void *localvar1 = (void *)malloc(2048);
	printf("variable globalvar1 \t location: 0x%x\n", &globalvar1);
	printf("variable globalvar2 \t location: 0x%x\n", &globalvar2);
	printf("variable localvar1 \t location: 0x%x\n", &localvar1);
	mylibfoo();
	mylocalfoo();
	while(1);
	return(0);
}
