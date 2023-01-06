#include <iostream>
#include <fstream>
#include <thread>

#include <bits/stdc++.h>
#include <curl/curl.h>

#include "util/parser.h"

using namespace std;

const int number_of_site = 8;

SiteMap sm_array[number_of_site];
thread sm_thread[number_of_site]; 

void TestDownloadFlow () {
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
    //set up source;
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

    // int test_var = 3;
    // sm_array[test_var].SiteInit();
    // DownloadURLIntoFile(sm_array[test_var].source_url, sm_array[test_var].source_file_name + ".txt");
    // ParseSiteMap(sm_array[test_var].source_file_name, sm_array[test_var].source, sm_array[test_var]);

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

    return 0;   
}