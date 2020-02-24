#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include"get_cc_path.h"
struct Args{
	std::string command;
	std::vector<std::string> options;
	std::vector<std::string> packages;
	bool has_help(bool help=true,bool _h=true,bool __help=true){
		for(auto&o:options){
			if((help && o=="help") || (_h && o=="-h") || (__help && o=="--help")){
				return true;
			}
		}
		return false;
	}
	bool has_option(std::vector<std::string> op){
		for(auto&o:options){
			for(auto&e:op){
				if(o==e)return true;
			}
		}
		return false;
	}
};
void help(std::string usage,std::vector<std::vector<std::string>> commands,std::vector<std::vector<std::string>> options){
	std::cout << std::endl << "Usage:" << std::endl << "  " << usage << std::endl << std::endl;
	if(!commands.empty()) {
		std::cout << "Commands:" << std::endl;
		for (auto &c:commands) {
			std::cout << "  " << std::setw(28) << std::left << c[0] << '\t' << c[1] << std::endl;
		}
	}
	if(!options.empty()) {
		std::cout << "Options:" << std::endl;
		for (auto &c:options) {
			std::cout << "  " << std::setw(28) << std::left << c[0] << '\t' << c[1] << std::endl;
		}
	}
}
void help1(std::string command=""){
	std::string usage="cip <command> [options]";
	std::vector<std::vector<std::string>> commands;
	commands.push_back({"install","Install packages."});
	commands.push_back({"download","Download packages."});
	commands.push_back({"uninstall","Uninstall packages."});
	commands.push_back({"freeze","Output installed packages in requirements format."});
	commands.push_back({"list","List installed packages."});
	commands.push_back({"show","Show information about installed packages."});
	commands.push_back({"check","Verify installed packages have compatible dependencies."});
	commands.push_back({"config","Manage local and global configuration."});
	commands.push_back({"search","Search PyPI for packages."});
	commands.push_back({"wheel","Build wheels from your requirements."});
	commands.push_back({"hash","Compute hashes of package archives."});
	commands.push_back({"completion","A helper command used for command completion."});
	commands.push_back({"debug","Show information useful for debugging."});
	commands.push_back({"help","Show help for commands."});

	std::vector<std::vector<std::string>> options;
	if(command!="" && command!="help"){
		if(std::find_if(commands.begin(),commands.end(),[&command](std::vector<std::string>& c)->bool{return c[0]==command;})==commands.end()){
			std::cout << "ERROR: unknown command \"" << command << "\"" << std::endl;
		}
	}else{
		help(usage,commands,options);
	}
}

void help_install(){
	//-r --requirement
	//-U --upgrade
	//-h --help
	std::string usage="cip install [options] <requirement specifier> ...\n";
	usage+="cip install [options] -r <requirements file> ...\n";
	usage+="cip install [options] <archive url/path> ...";
	std::vector<std::vector<std::string>> commands;
	std::vector<std::vector<std::string>> options;
	options.push_back({"-r, --requirement <file>","Install from the given requirements file. This option can be used multiple times."});
	options.push_back({"-U, --upgrade","Upgrade all specified packages to the newest available version. The handling of dependencies depends on the upgrade-strategy used."});
	options.push_back({"-h, --help","Show help."});
	help(usage,commands,options);
}
void parse_install(Args& args){
	if(args.has_help()){
		help_install();
	}else{
		for(auto&p:args.packages){
			std::cout << p << std::endl;
		}
	}
}


Args parse_command(int argc,char** argv) noexcept(false){
	Args args;
	if(argc==1)help1();
	args.command=argv[1];
	for(int i=2;i<argc;i++){
		if(argv[i][0]=='-')
			args.options.emplace_back(argv[i]);
		else
			args.packages.emplace_back(argv[i]);
	}
	if(args.command=="help" || args.command=="-h" || args.command=="--help")
		help1();
	if(args.command=="install")
		parse_install(args);
	return args;
}

int main(int argc,char* argv[]){
    CC cc;
    auto dir=cc.visualstudio2017();
    std::cout << dir.include_path << std::endl;
    std::cout << dir.lib86_path << std::endl;
    std::cout << dir.lib64_path << std::endl;
    std::cout << dir.dll86_path << std::endl;
    std::cout << dir.dll64_path << std::endl;
	//parse_command(argc,argv);

	return 0;
}