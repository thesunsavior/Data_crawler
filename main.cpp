#include <iostream>
#include <fstream>
#include <thread>

#include <bits/stdc++.h>
#include <curl/curl.h>

#include "util/parser.h"

using namespace std;

const int number_of_site = 7;
const string input_name = "";
const int doc_range = 10;

SiteMap sm_array[number_of_site];
vector<News> news_db;
string input_text = "";
Doc input_doc;
Date threshhold;

struct compare
{
public:
    bool operator()(Doc &a, Doc &b)
    {
        return input_doc.cos_similarity(a) > input_doc.cos_similarity(b);
    }
};
void BootAll()
{
    int news_db_size;

    // load threshhold and newsdb size
    ifstream threshhold_file("parameter.txt");
    if (threshhold_file.good())
    {
        string line;
        getline(threshhold_file, line);
        threshhold.ImportFromString(line);

        getline(threshhold_file, line);
        news_db_size = stoi(line);
    }
    else
        threshhold.set_date(2002, 3, 3); // retrieve the whole db

    // load newsdb
    fstream data_file;
    data_file.open("news.dat", ios::in | ios::binary);
    if (data_file.good())
    {
        data_file.read(reinterpret_cast<char *>(&news_db), news_db_size * sizeof(News));
    }
    data_file.close();
}

void UpdateNewsDB()
{
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
            sm_thread[i] = thread(&ParseArticle, ref(sm_array[i]), ref(news_db), threshhold);
        }

        for (int i = 0; i < number_of_site; i++)
        {
            sm_thread[i].join();
            cout << "-------- Join " << sm_array[i].source_file_name << " index: " << i << endl;
        }
    }
}

void SaveAll()
{
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

    data_file.close();

    ofstream threshhold_file;
    threshhold_file.open("parameter.txt");
    threshhold_file << threshhold.ToString() << "\n";
    threshhold_file << news_db.size();
    threshhold_file.close();
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

    BootAll();

    UpdateNewsDB();

    SaveAll();

    //@todo: Cluster documents with bag of words approach between
    input_doc.ExtractTextToBOW(input_text);
    priority_queue<Doc, vector<Doc>, compare> pq;

    for (int i = 0; i < news_db.size(); i++)
    {
        Doc temp;
        temp.ExtractTextToBOW(news_db[i].text);
        if (!pq.empty())
        {
            if (pq.size() < doc_range)
            {
                pq.push(temp);
                continue;
            }

            if (input_doc.cos_similarity(temp) > input_doc.cos_similarity(pq.top()))
            {
                pq.pop();
                pq.push(temp);
            }
        }
        else
            pq.push(temp);
    }

    //@todo write to file the top 10 similar doc
    // string temp;
    // while (!pq.empty())
    // {
    //     temp = pq.top().GetRawText();
    //     pq.pop();
    // }
    return 0;
}