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
struct Package{
	std::string version;
	std::vector<std::string> include_files;
	std::vector<std::string> lib_files;
	std::vector<std::string> dll_files;
};
class PackageList{
	std::vector<Package> packages;
	void read(std::string path){
		
	}
	void write(std::string path){

	}
};

#endif  //CIP_7E4_2_15_PACKAGE_LIST_H_INCLUDED