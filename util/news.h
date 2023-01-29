#ifndef UTIL_NEWS_H
#define UTIL_NEWS_H

#include <iostream>
#include <fstream>

#include <bits/stdc++.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "helper.h"

struct News
{
    string text = "";
    string url = "";
    vector<string> keywords;
    vector<string> NLP_keywords;
    string publish_date = "";

    void Clear();
    void ParseNewsLine(string line);
};

void ExecNewsParsing(string source_file_without_ext, string result_file_without_ext, string mode);

#endif