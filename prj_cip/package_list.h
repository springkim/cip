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
#include"get_cc_path.h"
struct Package{
    std::string name;
	std::string version;
	std::vector<std::string> include_files;
	std::vector<std::string> lib_files;
	std::vector<std::string> dll_files;
};
class PackageList{
	std::vector<Package> packages;
	void read(CCDir dir){
        std::ifstream fin;
        fin.open(dir.compiler_path+"cip_list.json", std::ios::in);
        std::string str;
        str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(str, root) == false) {
            return ;
        }
        for(auto it=root.begin();it!=root.end();it++){
            std::cout << (*it)["name"].asString() << std::endl;
        }
	}
	void write(CCDir dir){
        Json::Value root;
        for(auto&p:packages){
            Json::Value item;
            item["version"]=p.version;
            for(auto&e:p.include_files){
                item["include"].append(e);
            }
            for(auto&e:p.lib_files){
                item["lib"].append(e);
            }
            for(auto&e:p.dll_files){
                item["dll"].append(e);
            }
            root[p.name]=item;
        }
        Json::StyledWriter writer;
        std::ofstream fout(dir.compiler_path+"cip_list.json",std::ios::out);
        fout << writer.write(root) << std::endl;
        fout.close();
	}
};

#endif  //CIP_7E4_2_15_PACKAGE_LIST_H_INCLUDED