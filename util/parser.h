#include <iostream>
#include <fstream>

#include <bits/stdc++.h>
#include <curl/curl.h>
#include <ctype.h>

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

struct Date
{

    void set_date(int year, int month, int day = -1)
    {
        this->month = month;
        this->year = year;
        this->day = day;
    }

    int distance(Date *other)
    {
        int res = abs(other->month - this->month) + abs(other->year - this->year) * 10;
        return res;
    }

    bool operator<(const Date &other) const
    {
        return (this->year - other.year) * 10 + (this->month - other.month) < 0;
    }

    bool operator()(const Date &me, const Date &you) const
    {
        return (me.year - you.year) * 10 + (me.month - you.month) > 0;
    }

private:
    int month = -1;
    int year = -1;
    int day = -1;
};

struct SiteMap
{
    Source source;
    string source_file_name;
    map<Date, string> date_url;
    string source_url;
    ofstream log_file;

    void AddToMap(string url, int year, int month, int day = -1)
    {
        Date key;
        key.set_date(year, month, day);
        date_url[key]=url;
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