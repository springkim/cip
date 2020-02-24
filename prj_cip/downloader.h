//
// Created by VIRNECT on 2020-02-24.
//

#ifndef CIP_DOWNLOADER_H
#define CIP_DOWNLOADER_H
#include"ispring/Web.h"
#include<regex>
#include<set>
class Downloader{
private:
    std::vector<std::string> GetPackageList(std::string url,std::string ext) {
        std::string base_url;
        if (url.substr(0, 7) == "http://" || url.substr(0, 8) == "https://") {
            int last=url.find_first_of("/\\", 9);
            base_url = url.substr(0, last);
        }

        std::vector<std::string> list;
        auto html = ispring::Web::GetHtml(url);
        std::regex tag_a("<a[^>]*>");
        std::regex tag_href("href=\"([^\"]*\\."+ext+")\"");
        std::sregex_iterator it(html.begin(), html.end(), tag_a);
        std::sregex_iterator end;
        while (it != end) {
            std::string a = (*it).str(0);
            std::sregex_iterator jt(a.begin(), a.end(), tag_href);
            while (jt != end) {
                std::string href = (*jt).str(1);
                if (url.substr(0, 7) != "http://" && url.substr(0, 8) != "https://") {
                    href = base_url + href;
                }
                list.push_back(base_url + href);
                jt++;
            }
            it++;
        }
        return list;
    }
public:
    void Find(std::string package){
        auto list=GetPackageList("https://github.com/springkim/cip/releases/tag/test", "tir");
        //1. 버전 없음   opencv
        //2. 버전 있음   opencv==3.4
        //3. 옵션 있음   opencv==3.4+cu100

        std::set<std::string> options;
        for(auto e:list){
            int slash=e.find_last_of("/");
            std::string cpackage=e.substr(slash+1,e.length()-slash);
            std::cout << cpackage << std::endl;
        }
    }
};
#endif //CIP_DOWNLOADER_H
