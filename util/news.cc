#include "news.h"

void News::ParseNewsLine(string line)
{

    if (line.substr(0, 4) == "<pr>")
    {
        // get headers
        int until = line.find(":");
        string field = line.substr(4, until - 4);
        string value = line.substr(until + 2);

        if (field == "url")
        {
            this->url = value;
            return;
        }
        else if (field == "body")
        {
            this->text = value;
            return;
        }
        else if (field == "NLP_keywords")
        {
            this->NLP_keywords = seperator(value, ',');
            return;
        }
        else if (field == "keywords")
        {
            this->keywords = seperator(value, ',');
            return;
        }
        else if (field == "Date")
        {
            this->publish_date = value;
        }
    }
    else
        this->text += line;
}

void News::Clear()
{
    this->url = "";
    this->text = "";
    this->publish_date = "";
    this->keywords.clear();
    this->NLP_keywords.clear();
}

void ExecNewsParsing(string source_file_without_ext, string result_file_without_ext, string mode)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        cout << ">>>>>>>>>>>>> Executing child process news parser <<<<<<<<<<<<<" << endl;
        string cmd = "python3 ../news_parser.py " + source_file_without_ext + ".txt " + result_file_without_ext + ".txt" + " " + mode;
        ShellExec(cmd.c_str());
        exit(0);
    }
    else if (pid > 0)
    {
        int status = 0;
        wait(&status);
    }
    else
    {
        cout << "!!!!!!!!!!  News parsing failed, Error on creating child process !!!!!!!!!!!! " << endl;
        exit(0);
    }
    cout << "=================== Child process done ====================" << endl;
}