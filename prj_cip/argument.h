//
// Created by VIRNECT on 2020-02-25.
//

#ifndef CIP_ARGUMENT_H
#define CIP_ARGUMENT_H

#include<string>
#include<vector>
#include<set>
struct Args{
    std::string command;
    std::vector<std::pair<std::string,std::string>> options;
    std::vector<std::string> packages;
    bool has_help(bool _help=true,bool _h=true,bool __help=true){
        for(auto&o:options){
            if((_help && o.first=="-help") || (_h && o.first=="-h") || (__help && o.first=="--help")){
                return true;
            }
        }
        return false;
    }
    bool has_option(std::vector<std::string> op){
        for(auto&o:options){
            for(auto&e:op){
                if(o.first==e)return true;
            }
        }
        return false;
    }
};


#endif //CIP_ARGUMENT_H
