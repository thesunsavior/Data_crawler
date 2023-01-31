#ifndef UTIL_HELPER_H
#define UTIL_HELPER_H

#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <ctype.h>

using namespace std;

struct Date
{

    void set_date(int year, int month, int day = 0)
    {
        this->month = month;
        this->year = year;
        this->day = day;
    }

    int compare(Date other)
    {
        int res = (this->month - other.month) * 31 + (this->year - other.year) * 377 + (this->day - other.day);
        return res;
    }

    bool operator<(const Date &other) const
    {
        return (this->year - other.year) * 377 + (this->month - other.month) * 31 + (this->day - other.day) < 0;
    }

    bool operator()(const Date &me, const Date &you) const
    {
        return (me.year - you.year) * 377 + (me.month - you.month) * 31 + (me.day - you.day) > 0;
    }

    string ToString()
    {
        string res = "";
        res += to_string(year);
        res += "-";
        res += to_string(month);
        res += "-";
        res += to_string(day);
        return res;
    }

    void ImportFromString(string s)
    {
        string temp;
        int pos;
        int year, day, month;

        pos = s.find("-");
        temp = s.substr(0, pos);
        year = stoi(temp);

        pos = s.find("-", pos + 1);
        temp = s.substr(temp.size() + 2, pos - temp.size() - 2);
        month = stoi(temp);

        temp = s.substr(pos + 1);
        day = stoi(temp);

        this->set_date(year, month, day);
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