//
// Created by VIRNECT on 2020-02-24.
//

#ifndef CIP_DOWNLOADER_H
#define CIP_DOWNLOADER_H
#include<ispring/Web.h>
#include<ispring/Basic.h>
#include<ispring/Console.h>
#include<ispring/Compression.h>
#include<regex>
#include<algorithm>
#include<set>
#include<chrono>
#include<thread>
#include<atomic>
#include"argument.h"
#include"ccpath.h"
#include "package_list.h"

#if defined(_WIN32) || defined(_WIN64)
#include<Windows.h>
#endif
extern Args args;
struct DownloadPackage{
    std::string libname;
    std::string version;
    std::set<std::string> options;
    std::string url;
    std::string GetVersion(){
        std::string v=version;
        for(auto&o:options){
            v+="+"+o;
        }
        return v;
    }
    bool match(DownloadPackage& p){
        if(libname!=p.libname)return false;
        if(version!="" && version!=p.version)return false;
        std::vector<std::string> ioptions;
        std::set_intersection(options.begin(),options.end(),p.options.begin(),p.options.end(),std::inserter(ioptions,ioptions.begin()));
        if((version=="" && p.options.size()==0) || std::max(options.size(),p.options.size())==ioptions.size()){
            return true;
        }
        return false;
    }
};

class Downloader{
private:
    std::vector<std::string> GetPackageList(std::string url,std::string ext) {
        std::string base_url;
        if (url.substr(0, 7) == "http://" || url.substr(0, 8) == "https://") {
            int last=url.find_first_of("/\\", 9);
            base_url = url.substr(0, last);
        }

        std::vector<std::string> list;
        auto html = ispring::Web::GetHtml(url);
        std::regex tag_a("<a[^>]*>");
        std::regex tag_href("href=\"([^\"]*\\."+ext+")\"");
        std::sregex_iterator it(html.begin(), html.end(), tag_a);
        std::sregex_iterator end;
        while (it != end) {
            std::string a = (*it).str(0);
            std::sregex_iterator jt(a.begin(), a.end(), tag_href);
            while (jt != end) {
                std::string href = (*jt).str(1);
                if (url.substr(0, 7) != "http://" && url.substr(0, 8) != "https://") {
                    href = base_url + href;
                }
                list.push_back(base_url + href);
                jt++;
            }
            it++;
        }
        return list;
    }
    DownloadPackage CLibParse(std::string package){
        DownloadPackage ret;
        std::vector<std::string> parsed=ispring::String::Tokenizer(package,"+");
        ret.libname=parsed[0];
        if(parsed.size()>1)
            ret.version=parsed[1];
        for(int i=2;i<parsed.size();i++)
            ret.options.insert(parsed[i]);
        return ret;
    }
    DownloadPackage LibParse(std::string package){
        DownloadPackage ret;
        auto pos=package.find("==");
        std::string opstr;
        ret.libname=package;
        if(pos!=std::string::npos){
            opstr=package.substr(pos+2,package.length()-pos-1);
            ret.libname=package.substr(0,pos);
        }
        std::vector<std::string> parsed=ispring::String::Tokenizer(opstr,"+");
        if(parsed.size()>0)
            ret.version=parsed[0];
        for(int i=1;i<parsed.size();i++)
            ret.options.insert(parsed[i]);
        return ret;
    }
    std::string get_compiler_for_download(std::string parsed_compiler){
        std::map<std::string,std::string> compiler;
#if defined(_WIN32) || defined(_WIN64)
        compiler.insert(std::make_pair("vs2019","msvc"));
        compiler.insert(std::make_pair("vs2017","msvc"));
        compiler.insert(std::make_pair("vs2015","msvc"));
        compiler.insert(std::make_pair("vs2013","msvc"));
        compiler.insert(std::make_pair("vs2012","msvc"));
        compiler.insert(std::make_pair("vs2010","msvc"));
        compiler.insert(std::make_pair("gnuc","gnuc"));
#elif defined(__linux__)
        compiler.insert(std::make_pair("gnuc","gnuc"));
#endif
        auto it=compiler.find(parsed_compiler);
        if(it==compiler.end())return "";
        else return it->second;
    }
    std::string get_compiler(){
        for(auto&e:args.options){
            if(e.first=="-c" || e.first=="--compiler"){
                return e.second;
            }
        }
        std::string compiler_name=get_default_compiler().compiler_name;
        if(compiler_name==""){
            std::cout << ispring::xout.light_red << "No compiler found." << ispring::xout.white << std::endl;
            exit(1);
        }
        std::cout << ispring::xout.light_yellow << "Compiler is not specified for installer. Use the default compiler." << ispring::xout.light_green <<compiler_name << ispring::xout.white << std::endl;
        return compiler_name;
    }
    CCDir MakeCCDir(std::string compiler){
        CCDir ccdir;
#if defined(_WIN32) || defined(_WIN64)
        if(compiler=="vs2019")ccdir=visualstudio2019();
        if(compiler=="vs2017")ccdir=visualstudio2017();
        if(compiler=="vs2015")ccdir=visualstudio2015();
        if(compiler=="vs2013")ccdir=visualstudio2013();
#endif
        if(compiler=="gnuc")ccdir=gnuc64();
        return ccdir;
    }
public:
    void Archive(){
        auto list=GetPackageList("https://github.com/springkim/dataspace/releases/tag/cip(test)", "tir");
        std::sort(list.begin(), list.end(), std::greater<std::string>());
        std::string compiler_original=get_compiler();
        std::string compiler = get_compiler_for_download(compiler_original);
        for(auto&e:list){
            if(e.find(compiler)!=std::string::npos){
                auto slash=e.find_last_of("/");
                std::cout << e.substr(slash,e.length()-slash) << std::endl;
            }
        }
    }
    void Find(std::string package,bool downloadonly=false){
        auto list=GetPackageList("https://github.com/springkim/dataspace/releases/tag/cip(test)", "tir");
        std::sort(list.begin(), list.end(), std::greater<std::string>());
        //1. 버전 없음   opencv
        //2. 버전 있음   opencv==3.4
        //3. 옵션 있음   opencv==3.4+cu100
        DownloadPackage inpack=LibParse(package);
        inpack.options.erase("win64");
        inpack.options.erase("linux");
        inpack.options.erase("msvc");
        inpack.options.erase("gnuc");
        std::string compiler_original=get_compiler();
        std::string compiler = get_compiler_for_download(compiler_original);
        if(compiler==""){
            std::cout << ispring::xout.red << "The specified compiler is not supported." << ispring::xout.white << std::endl;
            exit(1);
        }
        inpack.options.insert(compiler);
#if defined(_WIN32) || defined(_WIN64)
        inpack.options.insert("win64");
#elif defined(__linux__)
        inpack.options.insert("linux");
#endif
        bool match=false;
        for(auto e:list){
            int slash=e.find_last_of("/");
            std::string cpackage=e.substr(slash+1,e.length()-slash);
            DownloadPackage outpack=CLibParse(cpackage.substr(0,cpackage.find_last_of('.')));
            if(inpack.match(outpack)) {
                match=true;
                outpack.url=e;
                Download(outpack,compiler_original,downloadonly);
                break;
            }
        }
        if(match==false){
            std::cout << ispring::xout.red << "ERROR: Could not find a version that satisfies the requirement " << inpack.libname << std::endl;
            std::cout << "ERROR: No matching distribution found for " << inpack.libname;
            if(inpack.version!="")
                std::cout << "==" << inpack.version ;
            std::cout << ispring::xout.white << std::endl;
        }
    }
    void PrintCollectiog(DownloadPackage& package){
        std::cout << "Collecting " << package.libname << "==" << package.version;
        for(auto&e:package.options){
            std::cout << "+" << e;
        }
        std::cout << std::endl;
    }
    void PrintDownloading(DownloadPackage& package){
        auto slash=package.url.find_last_of('/');
        std::string packagename=package.url.substr(slash+1,package.url.length()-slash);
        std::cout << "  Downloading " << packagename << std::endl;
    }
    void PrintAlreadySatisfied(DownloadPackage& package) {
        std::cout << "Requirement already satisfied: " << package.libname << "(" << package.version << ")" << std::endl;
    }
    std::string GetTempPath(){
#if defined(_WIN32) || defined(_WIN64)
        char buffer[MAX_PATH]={0};
        ::GetTempPathA(MAX_PATH,buffer);
        return buffer;
#else
        return "/tmp/";
#endif
    }
    std::atomic_bool thread_stop = false;
    void downloadcli(char b='>',char c='<',int length=20) {
#if defined(_WIN32) || defined(_WIN64)
        std::string bs = "\b";
#elif defined(__linux__)
        std::string bs = "\033[1D";
#endif
        using namespace std::chrono_literals;
        std::cout << "[";
        for (int i = 0; i < length+1; i++)std::cout << " ";
        std::cout << "]";
        for (int i = 0; i < length+2; i++)std::cout << bs;
        fflush(stdout);
        while(true){
            if (thread_stop)break;
            std::cout << b; fflush(stdout);
            for (int i = 0; i < length; i++) {
                std::cout << bs << " " << b; fflush(stdout);
                std::this_thread::sleep_for(0.05s);
            }
            if (thread_stop)break;
            for (int i = 0; i < length; i++) {
                std::cout << bs << " " << bs << bs << c; fflush(stdout);
                std::this_thread::sleep_for(0.05s);
            }
            std::cout << bs; fflush(stdout);
        }
        for (int i = 0; i < length * 2; i++)std::cout << "\b";
        for (int i = 0; i < length * 2; i++)std::cout << " ";
        for (int i = 0; i < length * 2; i++)std::cout << "\b";
        std::cout << "[";
        for (int i = 0; i < length + 1; i++)std::cout << "/";
        std::cout << "]" << std::endl;
    }
    void Download(DownloadPackage package,std::string compiler,bool downloadonly=false){
        CCDir ccdir=MakeCCDir(compiler);
        PackageList packagelist;
        if(downloadonly==false) {
            packagelist.read(ccdir);
            if (packagelist.exist(package.libname, package.version)) {
                PrintAlreadySatisfied(package);
                return;
            }
            packagelist.erase(package.libname);
        }
        PrintCollectiog(package);
        PrintDownloading(package);
        thread_stop=false;
        bool quiet=args.has_option({"-q","--quiet"});
        std::thread dcli;
        if(!quiet)
            dcli=std::thread(&Downloader::downloadcli,this,'>','<',20);

        auto slash = package.url.find_last_of("\\/");
        std::string packagename = package.url.substr(slash + 1, package.url.length() - slash);
        std::string dir = GetTempPath() + package.libname +DSLASH;
        ispring::File::DirectoryMake(dir);
        std::string file = dir + packagename;
        if(downloadonly){
            file=packagename;
        }
        ispring::Web::Download(package.url,file);

        thread_stop = true;
        if(!quiet)
            dcli.join();
        if(downloadonly) {
            std::cout << "Successfully downloaded " << packagename << std::endl;
            return;
        }
        std::cout << "Installing collected packages: " << package.libname << std::endl;
        ispring::File::DirectoryErase(file.substr(0,file.find_last_of('.')));
        ispring::Zip::Uncompress(file);
        std::string _3rdparty=file.substr(0,file.find_last_of('.')) + DSLASH + "3rdparty" + DSLASH;
        auto cutfilename=[](std::string& f)->void{
            f=ispring::String::GetNameOfFile(f);
        };
        if(get_compiler_for_download(compiler)=="msvc"){
            auto headers=ispring::File::FileList(_3rdparty+"include","*.*",true);
            auto drlib=ispring::File::FileList(_3rdparty+"lib","*.lib",false);
            auto dlib=ispring::File::FileList(_3rdparty+"lib" + DSLASH + "Debug","*.lib",false);
            auto rlib=ispring::File::FileList(_3rdparty+"lib" + DSLASH + "Release","*.lib",false);
            for_each(drlib.begin(),drlib.end(),cutfilename);
            for_each(rlib.begin(),rlib.end(),cutfilename);
            for_each(dlib.begin(),dlib.end(),cutfilename);
            std::string pragmacomment;
            pragmacomment+="#if defined(_DEBUG)\n";
            for(auto&e:dlib){
                pragmacomment+="#pragma comment(lib,\""+e+"\")\n";
            }
            pragmacomment+="#else\n";
            for(auto&e:rlib){
                pragmacomment+="#pragma comment(lib,\""+e+"\")\n";
            }
            pragmacomment+="#endif\n";
            for(auto&e:drlib){
                pragmacomment+="#pragma comment(lib,\""+e+"\")\n";
            }
            for(auto&e:headers){
                std::fstream fout(e,std::ios::app);
                fout << std::endl << pragmacomment << std::endl;
                fout.close();
            }
        }
        Package jsonpackage=ccdir.install(_3rdparty+"include",_3rdparty+"lib",_3rdparty+"bin",package.libname,package.GetVersion());

        packagelist.add(jsonpackage);
        packagelist.write(ccdir);
    }
    void Uninstall(std::string package){
        std::string compiler=get_compiler();
        CCDir ccdir=MakeCCDir(compiler);
        DownloadPackage inpack=LibParse(package);
        PackageList packagelist;
        packagelist.read(ccdir);
        if(packagelist.erase(inpack.libname)){
            std::cout << "Successfully uninstalled " << inpack.libname << std::endl;
        }else{
            std::cout << ispring::xout.yellow << "WARNING: Skipping " << inpack.libname << " as it is not installed." << ispring::xout.white << std::endl;
        }
        packagelist.write(ccdir);
    }
    void Freeze(){
        CCDir ccdir=MakeCCDir(get_compiler());
        PackageList packagelist;
        packagelist.read(ccdir);
        packagelist.freeze();
    }
};
#endif //CIP_DOWNLOADER_H
