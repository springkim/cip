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
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include"argument.h"
#include"downloader.h"
void init_hasargoption();
void parse_command(int argc,char** argv) noexcept(false);
void parse_install();
void parse_download();
void parse_uninstall();
void parse_freeze();
void parse_archive();

void help_install();
void help_download();
void help_uninstall();
void help_freeze();
void help_archive();
void help(std::string usage,std::vector<std::vector<std::string>> commands,std::vector<std::vector<std::string>> options);
void help_main(std::string command="");
void print_version();
#endif  //CIP_7E4_2_19_PARSER_H_INCLUDED