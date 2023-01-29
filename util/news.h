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
    unordered_map<string, int> bag_of_words;

    void Clear();
    void ParseNewsLine(string line);

    // calculate cos similarity;
    bool contains(string word);
    int get_value(string word);
    void insert(string word);
    int dot_product(News &doc2);
    double sq_euclid_length();
    double cos_similarity(News &doc2);

    // extract raw text to bag of words
    void ExtractTextToBOW();
};

void ExecNewsParsing(string source_file_without_ext, string result_file_without_ext, string mode);

#endif