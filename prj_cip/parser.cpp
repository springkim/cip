/*
*  parser.cpp
*  cip
*
*  Created by kimbomm on 2020. 2. 25...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#include"parser.h"
extern Args args;
extern std::set<std::string> hasargoption;
void print_version(){
    std::string version="0.0.1";
    std::cout << "cip " << version << std::endl;
}
void init_hasargoption(){
    hasargoption.insert("-c");
    hasargoption.insert("--compiler");
    hasargoption.insert("-r");
    hasargoption.insert("--requirement");
}
void parse_command(int argc,char** argv) noexcept(false){
    if(argc==1)help_main();
    args.command=argv[1];
    for(int i=2;i<argc;i++){
        if(argv[i][0]=='-') {
            std::pair<std::string,std::string> param;
            param.first=argv[i];
            if(hasargoption.find(param.first)!=hasargoption.end() && i+1<argc){
                param.second=argv[i+1];
                i++;
            }
            args.options.emplace_back(param);
        }else
            args.packages.emplace_back(argv[i]);
    }
    if(args.command=="help" || args.command=="-h" || args.command=="--help")
        help_main();
    if(args.command=="-V" || args.command=="--version")
        print_version();
    if(args.command=="install")
        parse_install();
    if(args.command=="download")
        parse_download();
    if(args.command=="uninstall")
        parse_uninstall();
    if(args.command=="freeze")
        parse_freeze();
    if(args.command=="archive")
        parse_archive();
}
void parse_install(){
    if(args.has_help()){
        help_install();
    }else{
        if(args.has_option({"-r","--requirement"})){
            std::string file=args.get_option_value({"-r","--requirement"});
            std::fstream fin(file,std::ios::in);
            if(fin.is_open()==false){
                std::cout << ispring::xout.red << "ERROR: Could not open requirements file: " << file << std::endl;
                exit(1);
            }
            while(!fin.eof()){
                std::string line;
                std::getline(fin,line);
                if(line.empty())continue;
                args.packages.push_back(line);
            }
            fin.close();
        }
        for(auto&p:args.packages){
            Downloader loader;
            loader.Find(p);
        }
    }
}
void parse_download(){
    if(args.has_help()){
        help_download();
    }else{
        if(args.has_option({"-r","--requirement"})){
            std::string file=args.get_option_value({"-r","--requirement"});
            std::fstream fin(file,std::ios::in);
            if(fin.is_open()==false){
                std::cout << ispring::xout.red << "ERROR: Could not open requirements file: " << file << std::endl;
                exit(1);
            }
            while(!fin.eof()){
                std::string line;
                std::getline(fin,line);
                if(line.empty())continue;
                args.packages.push_back(line);
            }
            fin.close();
        }
        for(auto&p:args.packages){
            Downloader loader;
            loader.Find(p,true);
        }
    }
}
void parse_uninstall(){
    if(args.has_help()){
        help_uninstall();
    }else{
        if(args.has_option({"-r","--requirement"})){
            std::string file=args.get_option_value({"-r","--requirement"});
            std::fstream fin(file,std::ios::in);
            if(fin.is_open()==false){
                std::cout << ispring::xout.red << "ERROR: Could not open requirements file: " << file << std::endl;
                exit(1);
            }
            while(!fin.eof()){
                std::string line;
                std::getline(fin,line);
                if(line.empty())continue;
                args.packages.push_back(line);
            }
            fin.close();
        }
        for(auto&p:args.packages){
            Downloader loader;
            loader.Uninstall(p);
        }
    }
}
void parse_freeze(){
    if(args.has_help()){
        help_freeze();
    }else {
        Downloader loader;
        loader.Freeze();
    }
}
void parse_archive(){
    if(args.has_help()){
        help_archive();
    }else {
        Downloader loader;
        loader.Freeze();
    }
}

void help_install(){
    //-c --compiler <compiler>
    //-r --requirement <file>
    //-U --upgrade
    //-h --help
    std::string usage="cip install [options] <requirement specifier> ...\n";
    usage+="  cip install [options] -r <requirements file> ...\n";
    usage+="  cip install [options] <archive url/path> ...";
    std::vector<std::vector<std::string>> commands;
    std::vector<std::vector<std::string>> options;
#if defined(_WIN32) || defined(_WIN64)
    options.push_back({"-c, --compiler <vs2019,vs2017,vs2015,vs2013,vs2012,vs2010,gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#elif defined(__linux__)
    options.push_back({"-c, --compiler <gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#endif
    options.push_back({"-r, --requirement <file>","Install from the given requirements file. This option can be used multiple times."});
    options.push_back({"-U, --upgrade","Upgrade all specified packages to the newest available version. The handling of dependencies depends on the upgrade-strategy used."});
    options.push_back({"-h, --help","Show help."});
    help(usage,commands,options);
}
void help_download(){
    //-c --compiler <compiler>
    //-r --requirement <file>
    //-h --help
    std::string usage="cip download [options] <requirement specifier> ...\n";
    usage+="  cip download [options] -r <requirements file> ...\n";
    usage+="  cip download [options] <archive url/path> ...";
    std::vector<std::vector<std::string>> commands;
    std::vector<std::vector<std::string>> options;
#if defined(_WIN32) || defined(_WIN64)
    options.push_back({"-c, --compiler <vs2019,vs2017,vs2015,vs2013,vs2012,vs2010,gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#elif defined(__linux__)
    options.push_back({"-c, --compiler <gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#endif
    options.push_back({"-r, --requirement <file>","Install from the given requirements file. This option can be used multiple times."});
    options.push_back({"-h, --help","Show help."});
    help(usage,commands,options);
}
void help_uninstall(){
    //-c --compiler <compiler>
    //-r --requirement <file>
    //-h --help
    std::string usage="cip uninstall [options] <requirement specifier> ...\n";
    usage+="  cip uninstall [options] -r <requirements file> ...\n";
    usage+="  cip uninstall [options] <archive url/path> ...";
    std::vector<std::vector<std::string>> commands;
    std::vector<std::vector<std::string>> options;
#if defined(_WIN32) || defined(_WIN64)
    options.push_back({"-c, --compiler <vs2019,vs2017,vs2015,vs2013,vs2012,vs2010,gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#elif defined(__linux__)
    options.push_back({"-c, --compiler <gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#endif
    options.push_back({"-r, --requirement <file>","Install from the given requirements file. This option can be used multiple times."});
    options.push_back({"-h, --help","Show help."});
    help(usage,commands,options);
}
void help_freeze(){
    //-c --compiler <compiler>
    //-h --help
    std::string usage="cip freeze [options]\n";
    std::vector<std::vector<std::string>> commands;
    std::vector<std::vector<std::string>> options;
#if defined(_WIN32) || defined(_WIN64)
    options.push_back({"-c, --compiler <vs2019,vs2017,vs2015,vs2013,vs2012,vs2010,gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#elif defined(__linux__)
    options.push_back({"-c, --compiler <gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#endif
    options.push_back({"-h, --help","Show help."});
    help(usage,commands,options);
}
void help_archive(){
    //-c --compiler <compiler>
    //-h --help
    std::string usage="cip archive [options]\n";
    std::vector<std::vector<std::string>> commands;
    std::vector<std::vector<std::string>> options;
#if defined(_WIN32) || defined(_WIN64)
    options.push_back({"-c, --compiler <vs2019,vs2017,vs2015,vs2013,vs2012,vs2010,gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#elif defined(__linux__)
    options.push_back({"-c, --compiler <gnuc>","Specify the compiler. If not used, it will be installed on the default compiler."});
#endif
    options.push_back({"-h, --help","Show help."});
    help(usage,commands,options);
}

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
void help_main(std::string command){
    std::string usage="cip <command> [options]";
    std::vector<std::vector<std::string>> commands;
    commands.push_back({"install","Install packages."});
    commands.push_back({"download","Download packages."});
    commands.push_back({"uninstall","Uninstall packages."});
    commands.push_back({"freeze","Output installed packages in requirements format."});
    commands.push_back({"archive","List installable packages."});
    commands.push_back({"help","Show help for commands."});

    std::vector<std::vector<std::string>> options;
    options.push_back({"-h, --help","Show help."});
    options.push_back({"-V, --version","Show version and exit."});
    options.push_back({"-q, --quiet","Give less output. Option is additive, and can be used up to 3 times (corresponding to WARNING, ERROR, and CRITICAL logging levels)."});
    if(command!="" && command!="help"){
        if(std::find_if(commands.begin(),commands.end(),[&command](std::vector<std::string>& c)->bool{return c[0]==command;})==commands.end()){
            std::cout << "ERROR: unknown command \"" << command << "\"" << std::endl;
        }
    }else{
        help(usage,commands,options);
    }
}