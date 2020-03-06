#include<ispring/System.h>
#include"ccpath.h"
#include"downloader.h"
#include"parser.h"


int main(int argc,char* argv[]){
    if(!ispring::OS::isAdmin()){
        std::cout << ispring::xout.light_red << "cip requires admin privileges to run" << ispring::xout.white << std::endl;
        return 1;
    }
    init_hasargoption();
	parse_command(argc,argv);
	return 0;
}