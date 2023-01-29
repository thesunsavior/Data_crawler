#ifndef UTIL_PARSER_H
#define UTIL_PARSER_H

#include <iostream>
#include <fstream>

#include <bits/stdc++.h>
#include <curl/curl.h>
#include <ctype.h>

#include "news.h"
#include "helper.h"

using namespace std;

const char ZingNews_source[] = "https://zingnews.vn/sitemap/sitemap.xml";
const char ThanhNien_source[] = "https://thanhnien.vn/sitemap.xml";
const char VietNamNet_source[] = "https://vietnamnet.vn/sitemap.xml";
const char VTC_source[] = "https://vtc.vn/sitemap.xml";
const char PhapLuat_source[] = "https://www.phapluatplus.vn/sitemap.xml";
const char TienPhong_source[] = "https://tienphong.vn/sitemap.xml";
const char NguoiDuaTin_source[] = "https://www.nguoiduatin.vn/sitemap/sitemap-index.xml";
const char DanTri_source[] = "https://dantri.com.vn/sitemaps/articles.xml";

enum Format
{
    yy,
    yy_mm,
    yy_mm_dd,
    dd_mm_yy,
    mm_yy, // ( 2 layer: yy then yy-mm-dd )
};

enum Source
{
    ZingNews,
    ThanhNien,
    VietNamNet,
    VTC,
    PhapLuat,
    TienPhong,
    DanTri,
    NguoiDuaTin
};

struct SiteMap
{
    Source source;
    string source_file_name;
    vector<pair<Date, string>> date_url;
    string source_url;
    ofstream log_file;
    vector<News> parse_news;

    void AddToMap(string url, int year, int month, int day = -1)
    {
        Date key;
        key.set_date(year, month, day);

        pair<Date, string> temp;
        temp.first = key;
        temp.second = url;

        date_url.push_back(temp);
    }

    // log to the site map log file with form "key : value"
    void Log(string field, string value)
    {
        log_file << field << " : " << value << "\n";
    }

    // log a single message to log file
    void Log(string message)
    {
        log_file << message << "\n";
    }

    void SiteInit();
};


// parse sitemap xml files with different date format 
void FormatParser(string sitemap_file_name, Format fm, SiteMap &site); // note that file name here is without extension

void ParseSiteMap(string sitemap_file_name, Source source, SiteMap &sitemap); // note that file name here is without extension

void ParseArticle(SiteMap &site, vector<News> &sm_news);
#endif