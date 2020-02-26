/*
*  argument.h
*  cip
*
*  Created by kimbomm on 2020. 2. 26...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#if !defined(CIP_7E4_2_1A_ARGUMENT_H_INCLUDED)
#define CIP_7E4_2_1A_ARGUMENT_H_INCLUDED
#include<string>
#include<vector>
#include<set>
#if defined(_WIN32) || defined(_WIN64)
#define DSLASH '\\'
#elif defined(__linux__)
#define DSLASH '/'
#endif

struct Args{
    std::string command;
    std::vector<std::pair<std::string,std::string>> options;
    std::vector<std::string> packages;
    bool has_help(bool _help=true,bool _h=true,bool __help=true);
    bool has_option(std::vector<std::string> op);
    std::string get_option_value(std::vector<std::string> op);
};


#endif  //CIP_7E4_2_1A_ARGUMENT_H_INCLUDED
