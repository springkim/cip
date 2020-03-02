/*
*  ccpath.cpp
*  cip
*
*  Created by kimbomm on 2020. 2. 26...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#include"ccpath.h"


#if defined(_WIN32) || defined(_WIN64)

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

CCDir visualstudio_new(int version) {
    CCDir dir;
    std::string cc = msvcwhere(version);
    if (cc.empty())return dir;
    dir.compiler_path = cc + "\\";
    dir.include_path = cc + "\\VC\\Tools\\MSVC\\";
    auto folders = ispring::File::FileList(dir.include_path, "", false, true);
    std::sort(folders.begin(), folders.end());
    dir.include_path = folders.back() + "\\include\\";
    dir.lib86_path = folders.back() + "\\lib\\x86\\";
    dir.lib64_path = folders.back() + "\\lib\\x64\\";
    char buffer[MAX_PATH];
    SHGetSpecialFolderPathA(nullptr, buffer, CSIDL_SYSTEM, FALSE);
    dir.dll64_path = std::string(buffer) + "\\";
    SHGetSpecialFolderPathA(nullptr, buffer, CSIDL_SYSTEMX86, FALSE);
    dir.dll86_path = std::string(buffer) + "\\";
    return dir;
}

CCDir visualstudio_old(int version) {
    CCDir dir;
    std::string cc = msvcwhere(version);
    if (cc.empty())return dir;
    dir.compiler_path = cc + "\\";
    dir.include_path = cc + "\\VC\\include\\";
    dir.lib86_path = cc + "\\VC\\lib\\";
    dir.lib64_path = cc + "\\VC\\lib\\amd64\\";
    char buffer[MAX_PATH];
    SHGetSpecialFolderPathA(nullptr, buffer, CSIDL_SYSTEM, FALSE);
    dir.dll64_path = std::string(buffer) + "\\";
    SHGetSpecialFolderPathA(nullptr, buffer, CSIDL_SYSTEMX86, FALSE);
    dir.dll86_path = std::string(buffer) + "\\";
    return dir;
}

CCDir visualstudio2019() {
    CCDir dir = visualstudio_new(16);
    dir.compiler_name = "vs2019";
    return dir;
}

CCDir visualstudio2017() {
    CCDir dir = visualstudio_new(15);
    dir.compiler_name = "vs2017";
    return dir;
}

CCDir visualstudio2015() {
    CCDir dir = visualstudio_old(14);
    dir.compiler_name = "vs2015";
    return dir;
}

CCDir visualstudio2013() {
    CCDir dir = visualstudio_old(12);
    dir.compiler_name = "vs2013";
    return dir;
}

CCDir gnuc64() {
    CCDir dir;
    std::string path;
    FILE *fp = _popen("where g++ 2> nul", "r");
    if (fp != nullptr) {
        char c;
        while (fread(&c, 1, 1, fp) == 1 && c != EOF) {
            path.push_back(c);
        }
        _pclose(fp);
    }
    path = path.substr(0, path.find_first_of('\n'));
    if (path.empty())return dir;
    dir.dll64_path = dir.compiler_path = path.substr(0, path.find_last_of("\\/"));
    dir.dll64_path += "\\";
    std::string root = dir.compiler_path.substr(0, dir.compiler_path.find_last_of("\\/") + 1);
    dir.compiler_path+="\\";
    dir.include_path = root + "x86_64-w64-mingw32\\include\\";
    dir.lib64_path = root + "x86_64-w64-mingw32\\lib\\";
    dir.compiler_name = "gnuc";
    return dir;
}

CCDir get_default_compiler() {
    CCDir dir;
    dir = visualstudio2019();
    if (dir.compiler_path != "")return dir;
    dir.clear();
    dir = visualstudio2017();
    if (dir.compiler_path != "")return dir;
    dir.clear();
    dir = visualstudio2015();
    if (dir.compiler_path != "")return dir;
    dir.clear();
    dir = visualstudio2013();
    if (dir.compiler_path != "")return dir;
    dir.clear();
    dir = gnuc64();
    if (dir.compiler_path != "")return dir;
    dir.clear();
    return dir;
}

#elif defined(__linux__)

CCDir gnuc64() {
    CCDir dir;
    dir.include_path="/usr/include/";
    dir.lib64_path="/usr/lib/";
    dir.dll64_path="/usr/lib/";
    dir.compiler_path="/usr/";
    return dir;
}
CCDir get_default_compiler(){
    CCDir dir;
    dir=gnuc64();
    if(dir.compiler_path!="")return dir;
    dir.clear();
    return dir;
}

#endif