
#include"ccpath.h"
#include"downloader.h"
#include"parser.h"


int main(int argc,char* argv[]){
    init_hasargoption();
	parse_command(argc,argv);
	return 0;
}