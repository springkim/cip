/*
*  parser.h
*  cip
*
*  Created by kimbomm on 2020. 2. 25...
*  Copyright 2020 kimbomm. All rights reserved.
*
*/
#if !defined(CIP_7E4_2_19_PARSER_H_INCLUDED)
#define CIP_7E4_2_19_PARSER_H_INCLUDED
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include"argument.h"
#include"downloader.h"
void init_hasargoption();
void parse_command(int argc,char** argv) noexcept(false);
void parse_install();

void help_install();
void help(std::string usage,std::vector<std::vector<std::string>> commands,std::vector<std::vector<std::string>> options);
void help1(std::string command="");

#endif  //CIP_7E4_2_19_PARSER_H_INCLUDED