#include "helper.h"

using namespace std;

string GetInputBetween(string line, string starter, string ender,int find_pos)
{
    // return the first string in "line" that stand between starter and ender
    string result = "";
    int start_pos = line.find(starter,find_pos);
    if (start_pos != string::npos)
        start_pos += starter.length();

    int end_pos = line.find(ender,find_pos);

    if (start_pos == string::npos || end_pos == string::npos)
        return "";

    for (int i = start_pos; i < end_pos; i++)
    {
        result += line[i];
    }

    return result;
}

void DownloadURLIntoFile(string url, string file_name_with_ext)
{
    // download a file from url and push it into file_name_with_ext
    // deal with c stuff
    char *c_url = new char[url.length() + 1];
    strcpy(c_url, url.c_str());

    char *c_file_name = new char[file_name_with_ext.length() + 1];
    strcpy(c_file_name, file_name_with_ext.c_str());

    FILE *file_write = fopen(c_file_name, "w");

    // start downloader
    CURL *downloader = curl_easy_init();
    curl_easy_setopt(downloader, CURLOPT_URL, c_url);
    curl_easy_setopt(downloader, CURLOPT_WRITEDATA, file_write);
    curl_easy_setopt(downloader, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(downloader, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(downloader, CURLOPT_VERBOSE, 1L);

    // log result to console
    cout << "=================================== Downloading " << url << " ==================================="<<endl;
    int result = curl_easy_perform(downloader);

    if (result == CURLE_OK)
        cout << "File downloaded" << endl;
    else
        cout << "Error has occur, code: " << result;

    curl_easy_cleanup(downloader);
    cout << "=============================================================================================="<<endl;

    fclose(file_write);
}