#include <iostream>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <ctype.h>

using namespace std;

// return the first string in "line" that stand between starter and ender
string GetInputBetween(string line, string starter, string ender,int find_pos);

// download a file from url and push it into file_name_with_ext
void DownloadURLIntoFile(string url, string file_name_with_ext); 