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
    char text[999999];
    void Clear();
    void ImportFromString(string text);
};

struct Doc
{
private:
    string raw_text;
    unordered_map<string, int> bag_of_words;

public:
    // calculate cos similarity;
    bool contains(string word);
    int get_value(string word);
    void insert(string word);
    int dot_product(Doc doc2);
    double sq_euclid_length();
    double cos_similarity(Doc doc2);

    // extract raw text to bag of words
    void ExtractTextToBOW(string text);
    string GetRawText();
};

void ExecNewsParsing(string source_file_without_ext, string result_file_without_ext, string mode);
void ParseNewsLine(string line, string &temp);

#endif