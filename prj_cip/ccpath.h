/*
*  ccpath.h
*  cip
*
*  Created by kimbomm on 2020. 2. 26...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#if !defined(CIP_7E4_2_1A_CCPATH_H_INCLUDED)
#define CIP_7E4_2_1A_CCPATH_H_INCLUDED



#include<iostream>
#include <fstream>
#include<string>
#include<cstdio>

#if defined(_WIN32) || defined(_WIN64)

#include<Windows.h>
#include<urlmon.h> //URLDownloadToFileA
#include <Shlwapi.h>
#include<shlobj.h>


#endif
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "urlmon.lib")

#include"ispring/File.h"
struct Package{
    std::string name;
    std::string version;
    std::vector<std::string> include_files;
    std::vector<std::string> lib_files;
    std::vector<std::string> dll_files;
};
struct CCDir {
    std::string include_path;
    std::string lib86_path;
    std::string lib64_path;
    std::string dll86_path;
    std::string dll64_path;
    std::string compiler_path;
    std::string compiler_name;
    void clear() {
        include_path = "";
        lib86_path = "";
        lib64_path = "";
        dll86_path = "";
        dll64_path = "";
        compiler_path = "";
        compiler_name="";
    }
    Package install(std::string include,std::string lib,std::string bin,std::string libname,std::string version){
        Package package;
        package.name=libname;
        package.version=version;
        auto headerfiles=ispring::File::FileList(include,"*.*",true);
        for(auto h:headerfiles) {
            size_t pos = h.find("include") + 8;
            package.include_files.push_back(include_path+h.substr(pos, h.length() - pos));
        }

        ispring::File::DirectoryCopy(include,include_path);


        auto libfiles=ispring::File::FileList(lib,"*.*",true);
        for(auto&libfile:libfiles){
            ispring::File::FileCopy(libfile,lib64_path+ispring::String::GetNameOfFile(libfile));
            package.lib_files.push_back(lib64_path+ispring::String::GetNameOfFile(libfile));
        }
        auto binfiles=ispring::File::FileList(bin,"*.*",true);
        for(auto&binfile:binfiles){
            ispring::File::FileCopy(binfile,dll64_path+ispring::String::GetNameOfFile(binfile));
            package.dll_files.push_back(dll64_path+ispring::String::GetNameOfFile(binfile));
        }
        return package;
    }
};

#if defined(_WIN32) || defined(_WIN64)

std::string msvcwhere(int version) ;
CCDir visualstudio_new(int version);
CCDir visualstudio_old(int version) ;
CCDir visualstudio2019() ;
CCDir visualstudio2017() ;
CCDir visualstudio2015();
CCDir visualstudio2013();
CCDir gnuc64() ;
CCDir get_default_compiler() ;

#elif defined(__linux__)

CCDir gnuc64() ;
CCDir get_default_compiler();

#endif

#endif  //CIP_7E4_2_1A_CCPATH_H_INCLUDED