#ifndef UTIL_HELPER_H
#define UTIL_HELPER_H

#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <ctype.h>

using namespace std;

struct Date
{

    void set_date(int year, int month, int day = -1)
    {
        this->month = month;
        this->year = year;
        this->day = day;
    }

    int compare(Date other)
    {
        int res = (this->month - other.month) + (this->year - other.year) * 12;
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

// return the first string in "line" that stand between starter and ender
string GetInputBetween(string line, string starter, string ender,int find_pos);

// download a file from url and push it into file_name_with_ext
void DownloadURLIntoFile(string url, string file_name_with_ext);

// parse parameter separated with a separator
vector<string> seperator(string line, char seperator);

// exec shell command
string ShellExec(const char *cmd);

#endif