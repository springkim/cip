//
// Created by VIRNECT on 2020-02-24.
//

#ifndef CIP_DOWNLOADER_H
#define CIP_DOWNLOADER_H
#include<ispring/Web.h>
#include<ispring/Basic.h>
#include<ispring/Console.h>
#include<regex>
#include<algorithm>
#include<set>
#include<chrono>
#include<thread>
#include<atomic>
#include "argument.h"
#if defined(_WIN32) || defined(_WIN64)
#include<Windows.h>
#endif
extern Args args;
struct DownloadPackage{
    std::string libname;
    std::string version;
    std::set<std::string> options;
    std::string url;
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
                return get_compiler_for_download(e.second);
            }
        }
        return "";
    }
public:
    void Find(std::string package){
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

        std::string compiler = get_compiler();
        inpack.options.insert(compiler);

        bool match=false;
        for(auto e:list){
            int slash=e.find_last_of("/");
            std::string cpackage=e.substr(slash+1,e.length()-slash);
            DownloadPackage outpack=CLibParse(cpackage.substr(0,cpackage.find_last_of('.')));
            if(inpack.match(outpack)) {
                match=true;
                outpack.url=e;
                Download(outpack);
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
    void Download(DownloadPackage package){
        PrintCollectiog(package);
        PrintDownloading(package);
        thread_stop=false;
        std::thread dcli(&Downloader::downloadcli,this,'>','<',20);
        {
            auto slash = package.url.find_last_of('/');
            std::string packagename = package.url.substr(slash + 1, package.url.length() - slash);
            std::string file = GetTempPath() + packagename;
            ispring::Web::Download(package.url,file);
        }
        thread_stop = true;
        dcli.join();
        std::cout << "Installing collected packages: " << package.libname << std::endl;
        //ispring::Web::Download(package.url,)
        //다운로드 -> 어디? 임시폴더
        //압축풀기 -> 임시폴더
        //pragma comment 적용하기 pdb도 같이 옮겨야 함.
        //리스트에 적으면서 복사

    }
};
#endif //CIP_DOWNLOADER_H
