/*
*  argument.cpp
*  cip
*
*  Created by kimbomm on 2020. 2. 26...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#include"argument.h"
Args args;
std::set<std::string> hasargoption;

bool Args::has_help(bool _help,bool _h,bool __help) {
    for(auto&o:options){
        if((_help && o.first=="-help") || (_h && o.first=="-h") || (__help && o.first=="--help")){
            return true;
        }
    }
    return false;
}
bool Args::has_option(std::vector<std::string> op){
    for(auto&o:options){
        for(auto&e:op){
            if(o.first==e)return true;
        }
    }
    return false;
}
std::string Args::get_option_value(std::vector<std::string> op){
    for(auto&o:options){
        for(auto&e:op){
            if(o.first==e)return o.second;
        }
    }
    return "";
}