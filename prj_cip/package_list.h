/*
*  package_list.h
*  cip
*
*  Created by kimbomm on 2020. 2. 21...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#if !defined(CIP_7E4_2_15_PACKAGE_LIST_H_INCLUDED)
#define CIP_7E4_2_15_PACKAGE_LIST_H_INCLUDED
#include<iostream>
#include<vector>
#include<string>
#include<json/json.h>
#include<set>
#include"ccpath.h"

class PackageList{
public:
	std::map<std::string,Package> packages;
	void read(CCDir dir){
        packages.clear();
        std::ifstream fin;
        fin.open(dir.compiler_path + "cip_list.json", std::ios::in);
        Json::CharReaderBuilder builder;
        Json::Value root;
        JSONCPP_STRING errs;
        if (Json::parseFromStream(builder,fin, &root,&errs) == false) {
            return;
        }
        for (auto it = root.begin(); it != root.end(); it++) {
            Package package;
            package.name=it.key().asString();
            package.version=(*it)["version"].asString();
            for (auto jt = (*it)["include"].begin(); jt != (*it)["include"].end(); jt++) {
                package.include_files.push_back(jt->asString());
            }
            for (auto jt = (*it)["lib"].begin(); jt != (*it)["lib"].end(); jt++) {
                package.lib_files.push_back(jt->asString());
            }
            for (auto jt = (*it)["dll"].begin(); jt != (*it)["dll"].end(); jt++) {
                package.dll_files.push_back(jt->asString());
            }
            packages.insert(std::make_pair(package.name,package));
        }
	}
	void write(CCDir dir){
        Json::Value root;
        for(auto&p:packages){
            Json::Value item;
            item["version"]=p.second.version;
            for(auto&e:p.second.include_files){
                item["include"].append(e);
            }
            for(auto&e:p.second.lib_files){
                item["lib"].append(e);
            }
            for(auto&e:p.second.dll_files){
                item["dll"].append(e);
            }
            root[p.second.name]=item;
        }
        Json::StyledWriter writer;
        std::ofstream fout(dir.compiler_path+"cip_list.json",std::ios::out);
        fout << writer.write(root) << std::endl;
        fout.close();
	}
	bool erase(std::string libname){
	    auto it=packages.find(libname);
        if(it!=packages.end()){
            for(auto&f:it->second.include_files)
                ispring::File::FileErase(f);
            for(auto&f:it->second.lib_files)
                ispring::File::FileErase(f);
            //for(auto&f:it->second.dll_files)
                //ispring::File::FileErase(f);
            packages.erase(it);
            return true;
        }
        return false;
	}
	void add(Package package){
        packages.insert(std::make_pair(package.name,package));
	}
	bool exist(std::string libname,std::string version=""){
        auto it=packages.find(libname);
        if(it!=packages.end()) {
            if(version=="")return true;
            return it->second.version.substr(0,version.length())==version;
        }
        return false;
	}
	void freeze(){
        for(auto&p:packages){
            std::cout << p.second.name << "==" << p.second.version << std::endl;
        }
	}
};

#endif  //CIP_7E4_2_15_PACKAGE_LIST_H_INCLUDED