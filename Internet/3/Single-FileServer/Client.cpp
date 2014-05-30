
#include "common.h"

using namespace std;



extern void connFileServer(char* , int );

int main(int argc, char **argv) {


    if( 3 != argc) {
        cerr<<"[-] usage: "<<argv[0]<<"  ip  port "<<endl;
        exit(-1);
    }
    connFileServer(argv[1], atoi(argv[2]));

}
