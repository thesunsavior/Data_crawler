#include <iostream>
#include <fstream>
#include <thread>

#include <bits/stdc++.h>
#include <curl/curl.h>

#include "util/parser.h"

using namespace std;

const int number_of_site = 7;
const string input_name = "";

SiteMap sm_array[number_of_site];
vector<News> news_db;
string input_text = "";

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
    ifstream input_file;
    input_file.open(input_name); //

    // read input text
    string temp;
    while (getline(input_file, temp))
    {
        input_text += temp;
    }

    input_file.close();

    // set up source;
    sm_array[0].source = ZingNews;
    sm_array[1].source = ThanhNien;
    sm_array[2].source = VietNamNet;
    sm_array[3].source = VTC;
    sm_array[4].source = PhapLuat;
    sm_array[5].source = TienPhong;
    sm_array[6].source = DanTri;
    sm_array[7].source = NguoiDuaTin; // we have some problem with this source

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

    //@todo : create a thread pool for easy multithreading
    // This function is temporarily ineffectively async
    {
        thread sm_thread[number_of_site];
        cout << "++++++++++++++++++++++ Parsing DB source +++++++++++++++++++++++++++" << endl;
        for (int i = 0; i < number_of_site; i++)
        {
            sm_thread[i] = thread(&ParseArticle, ref(sm_array[i]), ref(news_db));
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
        data_file.write(reinterpret_cast<char *>(&news_db), news_db.size() * sizeof(News));
        data_file.close();
    }
    else
        cerr << "error opening datafile to write " << endl;

    //@todo: Cluster documents with bag of words approach between
    Doc input_doc;
    input_doc.ExtractTextToBOW(input_text);

    for (int i = 0; i < news_db.size(); i++)
    {
        //
    }
    return 0;
}