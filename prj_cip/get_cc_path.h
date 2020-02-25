//
// Created by spring on 2020-02-24.
//

#ifndef CIP_GET_CC_PATH_H
#define CIP_GET_CC_PATH_H

#include<iostream>
#include <fstream>
#include<string>
#include<cstdio>
#if defined(__WIN32) || defined(__WIN64)
#include<Windows.h>
#include<urlmon.h> //URLDownloadToFileA
#include <Shlwapi.h>
#if defined(_MSC_VER)
#include<Shlobj_core.h>
#elif defined(__GNUC__)
#include<shlobj.h>
#endif
#endif
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "urlmon.lib")
#include"ispring/File.h"
struct CCDir{
    std::string include_path;
    std::string lib86_path;
    std::string lib64_path;
    std::string dll86_path;
    std::string dll64_path;
    std::string compiler_path;
};
#if defined(_WIN32) || defined(_WIN64)
class CC {
private:
    std::string msvcwhere(int version) {
        std::string path;
        char c_temp_path[MAX_PATH + 1];
        GetTempPathA(MAX_PATH, c_temp_path);
        std::string temp_path = c_temp_path;
        std::string exe = temp_path + "vswhere.exe";
        std::ifstream fin(exe, std::ifstream::ate | std::ifstream::binary);
        int result = 1;
        if (PathFileExistsA(exe.c_str()) == FALSE || static_cast<int>(fin.tellg()) != 455056)
            result = (URLDownloadToFileA(nullptr,
                                         "https://github.com/springkim/ispring/releases/download/bin/vswhere.exe",
                                         exe.c_str(), 0, 0) == S_OK);
        if (result != 0) {
            char buf[256] = {0};
            sprintf_s(buf, 256, "%s -legacy -version [%d.0,%d.0) -property installationPath", exe.c_str(), version,
                      version + 1);
            FILE *fp = _popen(buf, "r");
            if (fp != nullptr) {
                char c;
                while (fread(&c, 1, 1, fp) == 1 && c != EOF) {
                    path.push_back(c);
                }
                _pclose(fp);
            }
            path = path.substr(0, path.find_last_not_of("\\\n\t ") + 1);
        }
        return path;
    }
    CCDir visualstudio_new(int version){
        CCDir dir;
        std::string cc=msvcwhere(version);
        if(cc.empty())return dir;
        dir.compiler_path=cc+"\\";
        dir.include_path=cc+"\\VC\\Tools\\MSVC\\";
        auto folders=ispring::File::FileList(dir.include_path,"",false,true);
        std::sort(folders.begin(),folders.end());
        dir.include_path=folders.back() +"\\include\\";
        dir.lib86_path=folders.back()+"\\lib\\x86\\";
        dir.lib64_path=folders.back()+"\\lib\\x64\\";
        char buffer[MAX_PATH];
        SHGetSpecialFolderPathA(nullptr,buffer,CSIDL_SYSTEM,FALSE);
        dir.dll86_path=std::string(buffer)+"\\";
        SHGetSpecialFolderPathA(nullptr,buffer,CSIDL_SYSTEMX86,FALSE);
        dir.dll64_path=std::string(buffer)+"\\";
        return dir;
    }
    CCDir visualstudio_old(int version){
        CCDir dir;
        std::string cc=msvcwhere(version);
        if(cc.empty())return dir;
        dir.compiler_path=cc+"\\";
        dir.include_path=cc +"\\VC\\include\\";
        dir.lib86_path=cc+"\\VC\\lib\\";
        dir.lib64_path=cc+"\\VC\\lib\\amd64\\";
        char buffer[MAX_PATH];
        SHGetSpecialFolderPathA(nullptr,buffer,CSIDL_SYSTEM,FALSE);
        dir.dll86_path=std::string(buffer)+"\\";
        SHGetSpecialFolderPathA(nullptr,buffer,CSIDL_SYSTEMX86,FALSE);
        dir.dll64_path=std::string(buffer)+"\\";
        return dir;
    }
public:
    CCDir visualstudio2019(){
        return visualstudio_new(16);
    }
    CCDir visualstudio2017(){
        return visualstudio_new(15);
    }
    CCDir visualstudio2015(){
        return visualstudio_old(14);
    }
    CCDir visualstudio2013(){
        return visualstudio_old(12);
    }
};
#elif defined(__linux__)
class CC {
private:

public:
    CCDir gcc() {
        CCDir dir;
        dir.include_path="/usr/include/";
        dir.lib64_path="/usr/lib/";
        dir.dll64_path="/usr/lib/";
        dir.compiler_path="/usr/";
        return dir;
    }

};
#endif

#endif //CIP_GET_CC_PATH_H
