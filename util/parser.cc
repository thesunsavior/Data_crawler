#include "parser.h"


using namespace std;

void FormatParser(string sitemap_file_name, Format fm, SiteMap &site) // note that file name here is without extension
{
    ifstream sitemap;
    sitemap.open(sitemap_file_name+".txt");
    switch (fm)
    {
        case yy:
        {
            // if input format is yy then there must be another layer
            string line;
            while (getline (sitemap,line)) {

                int start_find_pos =0 ;
                while (line.find("<loc>", start_find_pos) != string::npos)
                {
                  string url = GetInputBetween(line, "<loc>", "</loc>",start_find_pos);

                  if (url!= "")
                      start_find_pos = line.find("</loc>",start_find_pos)+6;

                  string holder = "";
                  for (int i = 0; i < url.length(); i++)
                      if (isdigit(url[i]))
                      {
                          holder += url[i];
                      }

                  // Lines that does not contain the date does not matter
                  if (holder.length() < 4)
                      continue;

                  DownloadURLIntoFile(url, sitemap_file_name + "_next_layer.txt");
                  FormatParser(sitemap_file_name + "_next_layer", yy_mm_dd, site);
                }
            }
            break;
        }

        case yy_mm:
        {
            string line;
            while (getline(sitemap, line))
            {
                int start_find_pos =0 ;
                while (line.find("<loc>", start_find_pos) != string::npos)
                {
                  string url = GetInputBetween(line, "<loc>", "</loc>", start_find_pos);

                  if (url != "")
                      start_find_pos = line.find("</loc>", start_find_pos) + 6;

                  string holder = "";
                  for (int i = 0; i < url.length(); i++)
                      if (isdigit(url[i]))
                      {
                          holder += url[i];
                      }

                  // Lines that does not contain the date does not matter
                  if (holder.length() < 6)
                      continue;

                  string year = holder.substr(0, 4);
                  string month = holder.substr(4);
                  site.AddToMap(url, stoi(year), stoi(month));

                  site.Log("Year", year);
                  site.Log("Month", month);
                  site.Log("Url", url);
                  site.Log("======================================================");
                }
            }
            break;
        }

        case yy_mm_dd:
        {
            string line;
            while (getline(sitemap, line))
            {
                int start_find_pos =0 ;
                while (line.find("<loc>", start_find_pos) != string::npos)
                {
                    cout<< line <<endl;
                    string url = GetInputBetween(line, "<loc>", "</loc>",start_find_pos);

                    if (url!= "")
                        start_find_pos = line.find("</loc>",start_find_pos)+6;

                    string holder = "";
                    for (int i = 0; i < url.length(); i++)
                        if (isdigit(url[i]))
                        {
                          holder += url[i];
                        }

                    // Lines that does not contain the date does not matter
                    if (holder.length() < 8)
                        continue;

                    string year  = holder.substr(0, 4);
                    string month = holder.substr(4,2);
                    string day   = holder.substr(6);
                    site.AddToMap(url, stoi(year), stoi(month),stoi(day));

                    site.Log("Year", year);
                    site.Log("Month", month);
                    site.Log("day", day);
                    site.Log("Url", url);
                    site.Log("======================================================");
                }
            }
            break;
        }

        case mm_yy:
        {
            string line;
            while (getline(sitemap, line))
            {
                int start_find_pos =0 ;
                while (line.find("<loc>", start_find_pos) != string::npos)
                {
                    cout<< line <<endl;
                    string url = GetInputBetween(line, "<loc>", "</loc>",start_find_pos);

                    if (url!= "")
                        start_find_pos = line.find("</loc>",start_find_pos)+6;

                    string holder = "";
                    for (int i = 0; i < url.length(); i++)
                        if (isdigit(url[i]))
                        {
                          holder += url[i];
                        }

                    // Lines that does not contain the date does not matter
                    if (holder.length() < 6)
                        continue;

                    string year = holder.substr(2);
                    string month = holder.substr(0,2);
                    site.AddToMap(url, stoi(year), stoi(month));
                    site.Log("Year", year);
                    site.Log("Month", month);
                    site.Log("Url", url);
                    site.Log("======================================================");
                }
            }
            break;
        }

        default:
            break;
    }
}

void ParseSiteMap(string sitemap_file_name, Source source, SiteMap &sitemap)
{
    switch (source)
    {
        case ZingNews:
        {
            FormatParser (sitemap_file_name,yy_mm_dd,sitemap);
            break;
        }

        case ThanhNien:
        {
            FormatParser(sitemap_file_name,yy_mm,sitemap);
            break;
        }

        case VietNamNet:
        {
            FormatParser(sitemap_file_name, mm_yy, sitemap);
            break;
        }

        case VTC:
        {
            FormatParser(sitemap_file_name, yy, sitemap);
            break;
        }

        case PhapLuat:
        {
            FormatParser(sitemap_file_name, yy_mm, sitemap);
            break;
        }

        case TienPhong:
        {
            FormatParser(sitemap_file_name, yy_mm, sitemap);
            break;
        }

        case DanTri:
        {
            FormatParser(sitemap_file_name, yy_mm_dd, sitemap);
            break;
        }

        case NguoiDuaTin:
        {
            FormatParser(sitemap_file_name, yy_mm, sitemap);
            break;
        }

        default:
            break;
    }

}

void SiteMap::SiteInit()
{
    if (this->source == -1)
            return;

    switch (this->source)
    {
    case ZingNews:
    {
            source_file_name = "zing_sitemap";
            source_url = ZingNews_source;
            break;
    }

    case ThanhNien:
    {
            source_file_name = "thanhnien_sitemap";
            source_url = ThanhNien_source;
            break;
    }

    case VietNamNet:
    {
            source_file_name = "vietnamnet_sitemap";
            source_url = VietNamNet_source;
            break;
    }

    case VTC:
    {
            source_file_name = "vtc_sitemap";
            source_url = VTC_source;
            break;
    }

    case PhapLuat:
    {
            source_file_name = "phapluat_sitemap";
            source_url = PhapLuat_source;
            break;
    }

    case TienPhong:
    {
            source_file_name = "tienphong_sitemap";
            source_url = TienPhong_source;
            break;
    }

    case DanTri:
    {
            source_file_name = "dantri_sitemap";
            source_url = DanTri_source;
            break;
    }

    case NguoiDuaTin:
    {
            source_file_name = "nguoiduatin_sitemap";
            source_url = NguoiDuaTin_source;
            break;
    }

    default:
            source_file_name = "default_sitemap";
            source_url = ZingNews_source;
            break;
    }

    log_file.open("../log/" + source_file_name + "_log.txt");
}