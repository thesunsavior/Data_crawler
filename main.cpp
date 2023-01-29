#include <iostream>
#include <fstream>
#include <thread>

#include <bits/stdc++.h>
#include <curl/curl.h>

#include "util/parser.h"

using namespace std;

const int number_of_site = 8;

SiteMap sm_array[number_of_site];
vector<News> sm_news[number_of_site];

void TestDownloadFlow()
{
    char url[] = "https://thanhnien.vn/them-nan-nhan-bi-sap-bay-mua-dat-nen-vung-ven-post1538546.html";
    FILE *test = fopen("test.html", "w");

    CURL *downloader = curl_easy_init();
    curl_easy_setopt(downloader, CURLOPT_URL, url);
    curl_easy_setopt(downloader, CURLOPT_WRITEDATA, test);
    curl_easy_setopt(downloader, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(downloader, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(downloader, CURLOPT_VERBOSE, 1L);

    int result = curl_easy_perform(downloader);

    if (result == CURLE_OK)
        cout << "File downloaded" << endl;
    else
        cout << "Error has occur, code: " << result;
    curl_easy_cleanup(downloader);

    fclose(test);
}

int main()
{
    // set up source;
    sm_array[0].source = ZingNews;
    sm_array[1].source = ThanhNien;
    sm_array[2].source = VietNamNet;
    sm_array[3].source = VTC;
    sm_array[4].source = PhapLuat;
    sm_array[5].source = TienPhong;
    sm_array[6].source = DanTri;
    sm_array[7].source = NguoiDuaTin;

    // we have to sort each of the link of the sitemap by date
    // so that it is easier to find a topic that is similar

    // ================================ Sort sitemap by date ===============================
    // int test_var = 2;
    // sm_array[test_var].SiteInit();
    // DownloadURLIntoFile(sm_array[test_var].source_url, sm_array[test_var].source_file_name + ".txt");
    // ParseSiteMap(sm_array[test_var].source_file_name, sm_array[test_var].source, sm_array[test_var]);
    {
        thread sm_thread[number_of_site];
        for (int i = 0; i < number_of_site; i++)
        {
            sm_array[i].SiteInit();
            DownloadURLIntoFile(sm_array[i].source_url, sm_array[i].source_file_name + ".txt");
            sm_thread[i] = thread(&ParseSiteMap, sm_array[i].source_file_name, sm_array[i].source, ref(sm_array[i]));

            cout << ">>>>>>>>> Start Parsing " << sm_array[i].source_file_name << " index: " << i << endl;
        }

        for (int i = 0; i < number_of_site; i++)
        {
            sm_thread[i].join();
            sm_array[i].log_file.close();
            cout << "-------- Join " << sm_array[i].source_file_name << " index: " << i << endl;
        }
    }

    // ================================ Parse the major source ===============================
    // our main source is Thanh Nien
    // parsing news paper
    cout << "++++++++++++++++++++++ Parsing major source +++++++++++++++++++++++++++" << endl;
    Date threshhold;
    threshhold.set_date(2022, 12, 31);
    int count = 0;
    for (int i = 0; i < sm_array[1].date_url.size(); i++)
    {
        if (threshhold.compare(sm_array[1].date_url[i].first) >= 0)
            continue;

        DownloadURLIntoFile(sm_array[1].date_url[i].second, "Major_Source.txt");
        ExecNewsParsing("Major_Source", "Major_result", "1");
    }

    string line;
    News temp;
    ifstream file_parser;
    file_parser.open("Major_result");
    while (getline(file_parser, line))
    {
        if (line.find("<news>") != string::npos || line.find("</news>") != string::npos)
        {
            if (line.find("</news>") != string::npos)
            {
                sm_news[1].push_back(temp);
                temp.Clear();
            }

            continue;
        }

        temp.ParseNewsLine(line);
    }

    cout << "================================ Done parsing Major!!! ===============================" << endl;

    //@todo : create a thread pool for easy multithreading
    // This function is temporarily ineffectively async
    {
        thread sm_thread[number_of_site];
        cout << "++++++++++++++++++++++ Parsing DB source +++++++++++++++++++++++++++" << endl;
        for (int i = 0; i < number_of_site; i++)
        {
            sm_thread[i] = thread(&ParseArticle, ref(sm_array[i]), ref(sm_news[i]));
        }

        for (int i = 0; i < number_of_site; i++)
        {
            sm_thread[i].join();
            cout << "-------- Join " << sm_array[i].source_file_name << " index: " << i << endl;
        }
    }

    //@todo: write to .dat file all saved newspaper so we can load them later without re-parsing
    fstream data_file;
    data_file.open("news.dat", ios::out | ios::binary);
    if (data_file.is_open())
    {
        // calculate the number of to write

        data_file.write(reinterpret_cast<char *>(sm_news), sizeof(sm_news));
        data_file.close();
    }
    else
        cerr << "error opening datafile to write " << endl;

    //@todo: Cluster documents with bag of words approach between
    return 0;
}